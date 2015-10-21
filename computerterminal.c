//
//  https://www.codeeval.com/open_challenges/108/
//  computerterminal
//
//  Created by Stephan Warren on 12/30/14.
//
/*
 
 COMPUTER TERMINAL
 
 
 CHALLENGE DESCRIPTION:
 
 
 In this problem you are writing the software for a small terminal with a 10-row, 10-column display (perhaps for a point-of-sale terminal). Rows and columns are numbered 0 through 9. The character that introduces a control sequence is ^, the circumflex. The character (or in one case, the two characters) immediately following the control sequence introducer will direct your software in performing its special functions.
 
 Here is the complete list of control sequences you will need to interpret:
 ^c - clear the entire screen; the cursor row and column do not change
 ^h - move the cursor to row 0, column 0; the image on the screen is not changed
 ^b - move the cursor to the beginning of the current line; the cursor row does not change
 ^d - move the cursor down one row if possible; the cursor column does not change
 ^u - move the cursor up one row, if possible; the cursor column does not change
 ^l - move the cursor left one column, if possible; the cursor row does not change
 ^r - move the cursor right one column, if possible; the cursor row does not change
 ^e - erase characters to the right of, and including, the cursor column on the cursor's row; the cursor row and column do not change
 ^i - enter insert mode
 ^o - enter overwrite mode
 ^^ - write a circumflex (^) at the current cursor location, exactly as if it was not a special character; this is subject to the actions of the current mode (insert or overwrite)
 ^DD - move the cursor to the row and column specified; each D represents a decimal digit; the first D represents the new row number, and the second D represents the new column number
 
 No illegal control sequences will ever be sent to the terminal. The cursor cannot move outside the allowed screen locations (that is, between row 0, column 0 and row 9, column 9).
 When a normal character (not part of a control sequence) arrives at the terminal, it is displayed on the terminal screen in a manner that depends on the terminal mode. When the terminal is in overwrite mode (as it is when it is first turned on), the received character replaces the character at the cursor's location. But when the terminal is in insert mode, the characters to the right of and including the cursor's location are shifted right one column, and the new character is placed at the cursor's location; the character previously in the rightmost column of the cursor's row is lost.
 Regardless of the mode, the cursor is moved right one column, if possible.
 
 INPUT SAMPLE:
 
 Your program should accept as its first argument a path to a filename. Input example is the following
 
 ^h^c
 ^04^^
 ^13/ \^d^b  /   \
 ^u^d^d^l^l^l^l^l^l^l^l^l
 ^r^r^l^l^d<CodeEval >^l^l^d/^b \
 ^d^r^r^66/^b  \
 ^b^d   \ /
 ^d^l^lv^d^b===========^i^94O123456
 789^94A=======^u^u^u^u^u^u^l^l\^o^b^r/
 OUTPUT SAMPLE:
 
 Print results in the following way.
0123456789
0    ^
1   / \
2  /   \
3 /     \
4<CodeEval>
5 \     /
6  \   /
7   \ /
8    v
9====A=====

 
*/

#include <stdio.h>

#define DIM 10
char term[DIM][DIM + 1];

void clearScreen()
{
    for(char r = 0; r < DIM; r++) {
        for(char c = 0; c < DIM; c++) {
            term[r][c] = ' ';
        }
        term[r][DIM] = '\0';
    }
}

void incCursor(char * r, char * c)
{
    *c = *c + 1;
    if(*c > 9) *c = 9;
//    if(*c >= DIM) {
//        *c = 0;
//        *r = *r + 1;
//        printf("Newline by incCursor\n");
//        if(*r >= DIM) printf("inc issue with too many rows\n");
//    }
}

void showTerm(char i, char j)
{
    printf("   ");
    for(char r = 0; r < DIM; r++) {
        printf("%d", r);
    }
    putchar('\n');
    for(char r = 0; r < DIM; r++) {
        printf("%d: ", r);
        for(char c = 0; c < DIM; c++) {
            putchar(term[r][c]);
        }
        putchar('\n');
    }
    printf("   ");
    for(char r = 0; r < DIM; r++) {
        printf("%d", r);
    }
    putchar('\n');
    printf("current cursor = %d, %d\n", i, j);
}

int main(int argc, const char * argv[]) {

    char * ptr;
    FILE *file = fopen(argv[1], "r");
    char line[1024];
    
    char r, c;
    clearScreen();
    char insertmode = 0;
    r = c = 0;
    char i;
    while((ptr = fgets(line, 1024, file))) {
//        printf("line = *%s*\n", line);
        while(*ptr != '\0') {
            if(*ptr == '\n') {
                // do nothing
//                printf("Newline.\n");
//                r++;
//                c = 0;
//                if(r == DIM) {
//                    printf("Newline exceeds DIM\n");
//                }
            }
            else if(*ptr != '^') {
                if(insertmode) {
                    i = 10;
                    while(i >= c) {
                        term[r][i] = term[r][i-1];
                        i--;
                    }
//                    printf("(%d,%d): >%c<\n", r, c, *ptr);
                    term[r][c] = *ptr;
                }
                else {
//                    printf("(%d,%d): >%c<\n", r, c, *ptr);
                    term[r][c] = *ptr;
                }
                incCursor(&r, &c);
            } // end of if char
            else {
//                printf("Control Sequence = %c%c",*ptr, *(ptr+1));
                ptr++;
//                if(('0' <= *ptr) && (*ptr <= '9')) printf("%c\n", *(ptr+1));
//                else printf("\n");
                switch(*ptr) {

                    case 'c': // clear screen
                        clearScreen();
                        break;
                        
                    case 'h': // home
                        r = c = 0;
                        break;
                        
                    case 'b': // beginning of line
                        c = 0;
                        break;
                        
                    case 'd': // down
                        if(r < DIM - 1) r++;
                        break;
                        
                    case 'u': // up
                        if(r > 0) r--;
                        break;
                        
                    case 'l': // left
                        if(c > 0) c--;
                        break;
                        
                    case 'r': // right
                        if(c < DIM - 1) c++;
                        break;
                        
                    case 'e': // erase row to the right of cursor including col
                        for(i = c; i < DIM; i++) term[r][i] = ' ';
                        break;
                        
                    case 'i': // insert mode on
                        insertmode = !0;
//                        printf("Insert mode on\n");
                        break;
                        
                    case 'o': // overwrite mode
                        insertmode = 0;
//                        printf("Overwrite mode on\n");
                        break;
                        
                    case '^': // caret
                        term[r][c] = '^';
                        break;
                        
                    case '0': //
                    case '1': //
                    case '2': //
                    case '3': //
                    case '4': //
                    case '5': //
                    case '6': //
                    case '7': //
                    case '8': //
                    case '9': //
                        r = *ptr - '0';
                        ptr++;
                        c = *ptr - '0';
                        break;
                    default: //
                        printf("Something strange occurred\n");
                        break;
                } // switch
//                showTerm(r, c);
            } // else
            ptr++;
        } // while on line
    } // while !feof
//    showTerm(r, c);
    for(char r = 0; r < DIM; r++) {
//        printf("%d: ", r);
        for(char c = 0; c < DIM; c++) {
            putchar(term[r][c]);
        }
        putchar('\n');
    }
    return 0;
}






















