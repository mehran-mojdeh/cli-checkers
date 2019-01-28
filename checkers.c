#include <stdio.h>
#include <stdbool.h>

void draw_board();
bool check_win();
void do_turn();
int move(int, int, int, int);
int attack_availible();

//global vars
char board[8][8];
int turn = 0; //if even it's white turn

int main(){
    //pieces start state
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(i<3){ //white pieces
                if((i+j)%2) {   //is this a black square or white one
                    board[i][j] = '\0';
                } else board[i][j] = 'w';
            }
            if(i>4){ //black pieces
                if((i+j)%2) {   //is this a black square or white one
                    board[i][j] = '\0';
                } else board[i][j] = 'b';
            }

        }
    }
    
    //gameplay
    draw_board();
    while(!(check_win())){
        do_turn();
    }
    return 0;
}

//board visualization
void draw_board(){
    for(int i=0; i<8; i++){
        printf(" :");
        for(int j=0; j<8; j++){
            if(board[i][j]){
                printf(" %c :", board[i][j]);
            } else {
                printf("   :");
            }
        }
        printf("\n");
        //row number
        printf("%d:",i+1);
        //row seperator
        for(int j=0; j<8; j++) printf("___:");
        printf("\n");
    }
    //column number
    printf("   a   b   c   d   e   f   g   h\n");
}

bool check_win(){
    int white, black = 0;

    //counting white and black pieces
    for (int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if (board[i][j] == 'b' || board[i][j] == 'B') black++;
            if (board[i][j] == 'w' || board[i][j] == 'W') white++;
        }
    }

    if (white == 0) {
        printf("Black is winner!");
        return true;
    }
    if (black == 0) {
        printf("White is winner!");
        return true;
    }
    return false;
}

void do_turn(){
    int start_row, des_row;
    char start_col, des_col, sr;
    int flag=0;
    do{
        if(turn%2){
            printf("It's Blacks turn!\nWhat is your move?\n piece #: (ex. 6,b)\nrow:");
        } else{
            printf("It's Whites turn!\nWhat is your move?\npiece #: (ex. 6,b)\nrow:");
        }
        scanf("%s", &sr);       /*I don't know why but couldn't store*/
        start_row = sr -48;     /*start_row directly. I tried! :( */
        printf("col:");
        scanf("%s", &start_col);
        printf("\ndestination: (ex. 5,a)\nrow:");
        scanf("%d", &des_row);
        printf("col:");
        scanf("%s", &des_col);
        
        //validation
        
        flag = 0;
        
        //inputcheck
        if(start_row>8 || start_row<=0) flag++;
        if(start_col-97>=8 || start_col-97<0) flag++;
        if(des_row>8 || des_row<=0) flag++;
        if(des_col-97>=8 || des_col-97<0) flag++;
        //start piece validation
        if(turn%2){
            if(board[start_row-1][start_col-97] != 'b') flag++;
            if(board[start_row-1][start_col-97] == 'B') flag--;
        } else {
            if(board[start_row-1][start_col-97] != 'w') flag++;
            if(board[start_row-1][start_col-97] == 'W') flag--;
        }  
        //move validation
        if(turn%2){
            if (start_row-des_row != 1) flag++;
            if (start_col-des_col != 1 && start_col-des_col != -1) flag++;
            if(board[start_row-1][start_col-97] == 'B' && start_row-des_row == -1) flag--; //king
            if(board[des_row-1][des_col-97] == 'B' || board[des_row-1][des_col-97] == 'b') flag++;
            if(board[des_row-1][des_col-97] == 'w' && board[2*des_row-start_row][2*des_col-start_col]=='w') flag++;
        } else {
            if (start_row-des_row != -1) flag++;
            if (start_col-des_col != 1 && start_col-des_col != -1) flag++;
            if(board[start_row-1][start_col-97] == 'W' && start_row-des_row == 1) flag--; //king
            if(board[des_row-1][des_col-97] == 'W' || board[des_row-1][des_col-97] == 'w') flag++;
            if(board[des_row-1][des_col-97] == 'b' && board[2*des_row-start_row][2*des_col-start_col]=='b') flag++;
        }
        //test: printf("%d,%c\n%d,%c\n",start_row,start_col,des_row,des_col);
        if(flag) printf("***** Not valid inputs! *****\n");
        else{
            flag = move(start_row-1, start_col-97, des_row-1, des_col-97);
            if(flag==1) printf("***** Invalid move! *****\n");
            if(flag==2) printf("***** You must attack! *****\n");
        }
    }while(flag);
    turn++;
    
}

int move(int s_r, int s_c, int d_r, int d_c){
    printf("**move() called** %d,%d -> %d,%d\n", s_r+1,s_c+1,d_r+1,d_c+1);
    //test: printf("%d\n%d\n",2*d_r-s_r,2*d_c-s_c);
    if(turn%2){
        if(board[d_r][d_c] == 'w' || board[d_r][d_c] == 'W') {  //attack
            if(2*d_r-s_r>=0 && 2*d_c-s_c>=0 && 2*d_c-s_c<8){  
                if(board[2*d_r-s_r][2*d_c-s_c] == '\0'){
                    board[d_r][d_c] = '\0';
                    board[2*d_r-s_r][2*d_c-s_c] = board[s_r][s_c];
                    board[s_r][s_c] = '\0';
                    //become king
                    if(2*d_r-s_r == 0) board[2*d_r-s_r][2*d_c-s_c] = 'B';
                    
                    //continue attacking
                    if(board[2*d_r-s_r-1][2*d_c-s_c+1] == 'w' || board[2*d_r-s_r-1][2*d_c-s_c+1] == 'W'){
                        move(2*d_r-s_r,2*d_c-s_c,2*d_r-s_r-1,2*d_c-s_c+1);
                    }
                    if(board[2*d_r-s_r-1][2*d_c-s_c-1] == 'w' || board[2*d_r-s_r-1][2*d_c-s_c-1] == 'W'){
                        move(2*d_r-s_r,2*d_c-s_c,2*d_r-s_r-1,2*d_c-s_c-1);
                    }
                } else{
                    return 1;
                }
            } else{
                return 1;
            }
        } else if(attack_availible()){
            return 2;
        } else{
            if(board[d_r][d_c] == '\0') {                //simple move
                board[d_r][d_c] = board[s_r][s_c];
                board[s_r][s_c] = '\0';
                //become king
                if(d_r == 0) board[d_r][d_c] = 'B';
            } else {
                return 1;
            }
        }

    } else{
        if(board[d_r][d_c] == 'b' || board[d_r][d_c] == 'B') {  //attack
            if(2*d_r-s_r<8 && 2*d_c-s_c>=0 && 2*d_c-s_c<8){  
                if(board[2*d_r-s_r][2*d_c-s_c] == '\0'){
                    board[d_r][d_c] = '\0';
                    board[2*d_r-s_r][2*d_c-s_c] = board[s_r][s_c];  
                    board[s_r][s_c] = '\0';
                    //become king
                    if(2*d_r-s_r == 7) board[2*d_r-s_r][2*d_c-s_c] = 'W';
                    
                    //continue attacking
                    if(board[2*d_r-s_r+1][2*d_c-s_c+1] == 'b' || board[2*d_r-s_r+1][2*d_c-s_c+1] == 'B'){
                        move(2*d_r-s_r,2*d_c-s_c,2*d_r-s_r+1,2*d_c-s_c+1);
                    } else if(board[2*d_r-s_r+1][2*d_c-s_c-1] == 'b' || board[2*d_r-s_r+1][2*d_c-s_c-1] == 'B'){
                        move(2*d_r-s_r,2*d_c-s_c,2*d_r-s_r+1,2*d_c-s_c-1);
                    }
                } else{
                return 1;
                }
            } else{
                return 1;
            }
        } else if(attack_availible()){                  //checks if any attacks availible
            return 2;                                   //if not it will do the simple move
        } else{
            if(board[d_r][d_c] == '\0') {                //simple move
                board[d_r][d_c] = board[s_r][s_c];
                board[s_r][s_c] = '\0';
                //become king
                if(d_r == 7) board[d_r][d_c] = 'W';
            } else {
                return 1;
            }
        }
    }
    draw_board();
    return 0;
}

int attack_availible() {
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(turn%2){
                if(board[i][j]=='b' || board[i][j]=='B'){           //for every black piece
                    if(board[i-1][j-1]=='w' || board[i-1][j-1]=='W'){   //checking up-left
                        if(board[i-2][j-2]=='\0') return 1;
                    }
                    if(board[i-1][j+1]=='w' || board[i-1][j+1]=='W'){   //checking up-right
                        if(board[i-2][j+2]=='\0') return 1;
                    }
                    if (board[i][j]=='B') {                     //king
                        if(board[i+1][j-1]=='w' || board[i+1][j-1]=='W'){   //checking down-left
                            if(board[i+2][j-2]=='\0') return 1;
                        }
                        if(board[i+1][j+1]=='w' || board[i+1][j+1]=='W'){   //checking down-right
                            if(board[i+2][j+2]=='\0') return 1;
                        }
                    }
                    
                }
            } else {
                if(board[i][j]=='w' || board[i][j]=='W'){    //for every white piece
                    if(board[i+1][j-1]=='b' || board[i+1][j-1]=='B'){   //checking down-left
                        if(board[i+2][j-2]=='\0') return 1;
                    }
                    if(board[i+1][j+1]=='b' || board[i+1][j+1]=='B'){   //checking down-right
                        if(board[i+2][j+2]=='\0') return 1;
                    }
                    if(board[i][j]=='W'){                       //king
                        if(board[i-1][j-1]=='b' || board[i-1][j-1]=='B'){   //checking up-left
                            if(board[i+2][j-2]=='\0') return 1;
                        }
                        if(board[i-1][j+1]=='b' || board[i-1][j+1]=='B'){   //checking up-right
                            if(board[i-2][j+2]=='\0') return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}