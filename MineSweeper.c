#include <stdio.h>
#include <stdlib.h>

#define SIZE 10
#define MINES 10

// setup grids as global variables
char answerGrid[SIZE][SIZE];
char playGrid[SIZE][SIZE];

void printGrid();
int check(int rol, int col);
void reveal(int row, int col);
void flag(int row, int col);
int checkWin();

int main() {
    // initialize answers to 0s and play to hidden
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            answerGrid[i][j] = '0';
            playGrid[i][j] = '*';
        }
    }
    srand(2);
    int row = -1;
    int col = -1;
    for(int i = 0; i < MINES; i++) {
        // get unique random coordinates for each mine
        while(row == -1 || col == -1 || answerGrid[row][col] == 'M') {
            row = rand() % SIZE;
            col = rand() % SIZE;
        }
        answerGrid[row][col] = 'M';
        // ++ all surrounding tiles to show adjacent mine
        for(int j = row - 1; j < row + 2; j++)
            if(j >= 0 && j < SIZE)
                for(int k = col - 1; k < col + 2; k++)
                    if(k >= 0 && k < SIZE && answerGrid[j][k] != 'M' && !(j == row && k == col))
                        answerGrid[j][k]++;
    }
    // initialize the game
    printGrid();
    // main loop of gameplay
    char command;
    while(1) {
        // get command from AI
        printf("Enter 'c' for check cell, 'f' for flag cell.\n");
        printf("Enter command & cell row col: ");
        scanf("%c %d %d", &command, &row, &col);
        if(command == 'c') {
            // check the cell
            if(!check(row, col)) {
                printGrid();
                printf("You hit a mine, game over.\n");
                break;
            }
        }
        else if(command == 'f')
            // flag a hidden cell
            flag(row, col);
        printGrid();
        // see if the game is completed
        if(checkWin()) {
            printf("Congratulations! You win!");
            break;
        }
    }
}

// prints a grid to the console
void printGrid() {
    printf("\n");
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++)
            printf("%c  ", playGrid[i][j]);
        printf("\n");
    }
}

// checks the target cell for mines, etc.
int check(int row, int col) {
    // check if a mine has been hit
    if(answerGrid[row][col] == 'M') {
        playGrid[row][col] = answerGrid[row][col];
        return 0;
    }
    // reveal the cell and surrounding if applicable
    reveal(row, col);
    return 1;
}

// recursive function to reveal large sections at a time
void reveal(int row, int col) {
    // prevent infinite loops
    if(playGrid[row][col] != '*')
        return;
    // reveal the tile, only continue w adjacents if it = 0
    playGrid[row][col] = answerGrid[row][col];
    if(playGrid[row][col] != '0')
        return;
    // call reveal on all adjacent tiles
    for(int i = row - 1; i < row + 2; i++)
        if(i >= 0 && i < SIZE)
            for(int j = col - 1; j < col + 2; j++)
                if(j >= 0 && j < SIZE && !(i == row && j == col))
                    reveal(i, j);
}

// flags a hidden cell
void flag(int row, int col) {
    if(playGrid[row][col] == '*')
        playGrid[row][col] = 'F';
}

// count the number of hidden/flagged tiles, check if all safe tiles revealed
int checkWin() {
    int count = 0;
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if(playGrid[i][j] == '*' || playGrid[i][j] == 'F')
                count++;
        }
    }
    // returns a true value if all hidden tiles are mines
    if(count == MINES)
        return 1;
    else
        return 0;
}

