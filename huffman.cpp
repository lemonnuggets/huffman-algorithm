#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include<algorithm>

using namespace std;
const char* fileName = "original.txt";

class Cell{
    int score = -1;
    char label;
    Cell* zero = nullptr;
    Cell* one = nullptr;
    public:
    // Declares a cell with a label and score that points to no other Cells
    Cell(char l, int s){
        label = (char) l;
        score = s;
        zero = nullptr;
        one = nullptr;
    }
    // Declares a cell with a score that points to two other Cells (zero, one)
    Cell(int s, Cell* newZero, Cell *newOne){
        score = s;
        zero = newZero;
        one = newOne;
    }
    //Increments score by one.
    void incrementScore(){
        score++;
    }
    //Returns label.
    char getLabel(){
        return label;
    }
    //Returns score.
    int getScore(){
        return score;
    }
    //returns pointer to cell to the left.
    Cell* stepLeft(){
        return zero;
    }
    //returns pointer to cell to the right.
    Cell* stepRight(){
        return one;
    }
    friend ostream& operator<<(ostream& os, const Cell& cell);
    // void addToLeft(Cell* newZero){
    //     zero = newZero;
    // }
    // void addToRight(Cell* newOne){
    //     one = newOne;
    // }
};

//defines how Cell should behave with << operator
ostream& operator<<(ostream& os, const Cell& cell){
    if(cell.score || cell.label){
        os << "Score: " << cell.score << "\tLabel:" << cell.label << endl;
        if(cell.one != nullptr && cell.zero != nullptr){
            os << *cell.zero << *cell.one;
        }
        os << endl;
        return os;
    }
}

// Returns iterator to first match it finds. If no match is found, returns cells.end().
vector<Cell>::iterator findCellWithChar(vector<Cell> &cells, char ch){
    vector<Cell>::iterator cellCursor;
    for(cellCursor = cells.begin(); cellCursor != cells.end(); ++cellCursor){
        if(ch == cellCursor -> getLabel()){
            return cellCursor;
        }
    }
    return cellCursor;
}

// Returns true if score of c1 is greater than c2
bool compareScores(Cell c1, Cell c2){
    return c1.getScore() > c2.getScore();
}

Cell makeTree(vector<Cell> &cells){
    if(cells.size() > 1){
        Cell c1 = *(cells.end() - 1);
        cells.pop_back();
        Cell c2 = *(cells.end() - 1);
        cells.pop_back();
        Cell cell = Cell(c1.getScore() + c2.getScore(), &c1, &c2);
        auto it = upper_bound(cells.cbegin(), cells.cend(), cell, compareScores);
        cells.insert(it, cell);
        return makeTree(cells);
    }else{
        return cells[0];
    }
}

int main(){
    fstream myFile;
    myFile.open(fileName, ios::in);
    if(myFile.is_open()){
        char ch;
        vector<Cell> cells;
        while(myFile.get(ch)){
            vector<Cell>::iterator cell = findCellWithChar(cells, ch);
            if(cell != cells.end()){            // if a match is found it increments 
                                                // that score by 1
                cell -> incrementScore();
            }else{                              // if no match is found, new
                                                // cell is added to cells with a
                                                // score of 1
                cells.push_back(Cell(ch, 1));
            }
        }
        sort(cells.begin(), cells.end(), compareScores);    // vector cells is sorted in 
                                                            // descending order of score
                                                            // of cell.

        vector<Cell>::iterator cellCursor;
        for(cellCursor = cells.begin(); cellCursor != cells.end(); ++cellCursor){
            cout << *cellCursor;
        }
        Cell head = makeTree(cells);
        cout << head;
    }
    return 0;
}