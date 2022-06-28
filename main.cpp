#include <bits/stdc++.h>
using namespace std;
using Grid = vector<vector<char>>;
using Coords = vector<pair<int,int>>;
// Building the architecture
struct Player
{
    int id;
    int score;
    char symbol;
};
struct Move {
    int x;
    int y;
    int nextX;
    int nextY;
};
Player player1;
Player player2;
//initialize  Players
void initializePlayers(){
    player1.id=1;
    player1.score=2;
    player1.symbol='1';
    player2.id=2;
    player2.score=2;
    player2.symbol='0';
}
//to Build game board
Grid makeGrid(int rows,int cols){
    Grid grid;
    for(int i=0;i<rows;i++){
        vector<char> v;
        for(int j=0;j<cols;j++){
            v.push_back('-');
        }
        grid.push_back(v);
    }
    grid[0][0]=player1.symbol;
    grid[0][cols-1]=player2.symbol;
    grid[rows-1][0]=player1.symbol;
    grid[rows-1][cols-1]=player2.symbol;
    return grid;
}
// the game is over or not
bool isFinished(Grid grid){
    int maximum=grid[0].size()*grid.size();
    if(player1.score==0 || player2.score==0)
        return true;
    if(player1.score == maximum || player2.score == maximum)
        return true;
    if(player1.score +player2.score ==maximum)
        return true;
    return false;

}
//printing game board
void printGrid(Grid grid){
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}
// the cell inside the board or not
bool isInBoard(Grid grid,int x, int y) {
        return 0 <= x && x < grid.size() && 0 <= y && y < grid[0].size();
    }
// the cell is empty or not
bool isEmpty(Grid grid,int x, int y) {
        return grid[x][y]=='-';
    }
// getting the first radius of the cell ( the surrounding cells)
Coords getRadius(Grid grid,int x,int y){
            Coords coords;
            for (int nextX = x - 1; nextX <= x + 1; nextX++) {
            for (int nextY = y - 1; nextY <= y + 1; nextY++) {
                if(isInBoard(grid,nextX,nextY )&& (nextX!=x || nextY!=y))
                    coords.push_back(make_pair(nextX,nextY));
            }
            }
        return coords;
}
// the cell inside the radius or not
bool inRadius(Grid grid, int x,int y,int nextX,int nextY){
     Coords coords= getRadius(grid,x,y);
     for(int i=0;i<coords.size();i++){
        if(coords[i].first==nextX && coords[i].second==nextY)
        return true;
     }

     return false;
}
//getting the second radius of the cell ( the surrounding cells of the first radius)
Coords getDoubleRadius(Grid grid,int x,int y){
        Coords coords;
        for (int nextX = x - 2; nextX <= x + 2; nextX++) {
        for (int nextY = y - 2; nextY <= y + 2; nextY++) {
            if(isInBoard(grid,nextX,nextY )&& isEmpty(grid,nextX,nextY)&&(nextX!=x || nextY!=y)){
                    coords.push_back(make_pair(nextX,nextY));
                }

            }
            }
        return coords;
}
// getting all possible moves for all player's balls
vector<Move> getPossibleMoves(Grid grid,Player player){
    vector<Move> moves;
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {   if(grid[i][j]==player.symbol){
            Coords coords =getDoubleRadius(grid,i,j);
            for(int l=0;l<coords.size();l++){
             Move move={i,j,coords[l].first,coords[l].second};
             moves.push_back(move);
            }
            }

        }

    }
    return moves;
}
// can the cell move from one point to another or not
bool canMove(Grid grid, int x,int y,int nextX,int nextY,Player player){
    Coords coords= getDoubleRadius(grid,x,y);
    if(!isEmpty(grid,x,y)&& player.symbol==grid[x][y])
     for(int i=0;i<coords.size();i++){
        if(coords[i].first==nextX && coords[i].second==nextY)
        return true;
     }

     return false;

}
// converting the opponent balls in radius of the player ball
Grid convertBlobs(Grid grid,int x,int y,Player player){
    Coords coords = getRadius(grid,x,y);
   char oponent= player.id ==1 ? player2.symbol : player1.symbol;
    for(int i=0;i<coords.size();i++){
        if(grid[coords[i].first][coords[i].second]==oponent){
            grid[coords[i].first][coords[i].second]=player.symbol;
        }

     }
    return grid;

}
// get players Scores
int getScores(Grid grid,Player player){
    player.score=0;
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {   if(grid[i][j]==player.symbol){
                player.score++;
            }
            }

        }


    return player.score;

}
// the actual move happens here(moving the player ball from one point to another with respect to it's consequences
Grid makeMove(Grid grid, int x,int y,int nextX,int nextY,Player player){
    if(canMove(grid,x,y,nextX,nextY,player)){
        if(inRadius(grid,x,y,nextX,nextY)){
            grid[nextX][nextY]=player.symbol;
            grid=convertBlobs(grid,nextX,nextY,player);
        }
        else{
            grid[x][y]='-';
            grid[nextX][nextY]=player.symbol;
           grid= convertBlobs(grid,nextX,nextY,player);

        }

    }
        //updating players scores
        player1.score=getScores(grid,player1);
        player2.score=getScores(grid,player2);
    return grid;
}

//printing the players scores
void printScores(){
    cout<<"player1 Score: "<<player1.score<<endl<<"player2 Score: "<<player2.score<<endl;
}
//getting the winner if exists
int getWinner(){
    if(player1.score>player2.score)
        return player1.id;
    else if(player2.score>player1.score)
        return player2.id;
    else
        return 0;

}
const int  MAX= 100000,MIN= -100000;
//evaluating the board
int evaluate(Grid grid){
    if(isFinished(grid)){
        int winnerID = getWinner();
        if(winnerID == 2)
            return 10000;
        else if(winnerID ==0){
            return 0;
        }

        else
            return -10000;

    }
    return player2.score - player1.score;

}
//min max algorithm
int minimax(Grid grid ,int depth,bool isMax){
    if(depth==0)
        return evaluate(grid);

    vector<Move> possibleMoves = getPossibleMoves(grid,player1);
    vector<Move> possibleMoves2 = getPossibleMoves(grid,player2);
    if(possibleMoves.size()==0||possibleMoves2.size()==0)
        return evaluate(grid);
    if(isMax){
        int best= MIN;
        for(int i=0;i<possibleMoves2.size();i++){
        grid= makeMove(grid,possibleMoves2[i].x,possibleMoves2[i].y,possibleMoves2[i].nextX,possibleMoves2[i].nextY,player2);
        best=max(best,minimax(grid,depth-1,!isMax));
        }
        return best;
    }
    else {
        int best= MAX;
        for(int i=0;i<possibleMoves.size();i++){
        grid= makeMove(grid,possibleMoves[i].x,possibleMoves[i].y,possibleMoves[i].nextX,possibleMoves[i].nextY,player1);
        best=min(best,minimax(grid,depth-1,!isMax));
        }
        return best;
    }

}


//alpha beta algorithm
int alphaBeta(Grid grid ,int depth,bool isMax,int alpha , int beta){
    if(depth==0)
        return evaluate(grid);

    vector<Move> possibleMoves = getPossibleMoves(grid,player1);
    vector<Move> possibleMoves2 = getPossibleMoves(grid,player2);
    if(possibleMoves.size()==0||possibleMoves2.size()==0)
        return evaluate(grid);
    if(isMax){
        for(int i=0;i<possibleMoves2.size();i++){
        grid= makeMove(grid,possibleMoves2[i].x,possibleMoves2[i].y,possibleMoves2[i].nextX,possibleMoves2[i].nextY,player2);
        int score =alphaBeta(grid,depth-1,!isMax,alpha,beta);
        if (score >= beta) {
            return beta;
            }
        if (score > alpha) {
            alpha = score;
            }
        }
        return alpha;
    }
    else {
        for(int i=0;i<possibleMoves.size();i++){
        grid= makeMove(grid,possibleMoves[i].x,possibleMoves[i].y,possibleMoves[i].nextX,possibleMoves[i].nextY,player1);
        int score =alphaBeta(grid,depth-1,!isMax,alpha ,beta);
        if (score <= alpha) {
            return alpha;
            }
        if (score < beta) {
            beta = score;
            }
        }
        return beta;
    }



}
//find best move
Move findBestMove(Grid grid, int depth,Player player,int algo){
    int bestVal = MIN;
	Move bestMove;
	bestMove.x = -1;
	bestMove.y = -1;
    bestMove.nextX = -1;
	bestMove.nextY = -1;
    vector<Move> possibleMoves = getPossibleMoves(grid,player2);
   // cout<<possibleMoves.size()<<endl;
    for(int i=0;i<possibleMoves.size();i++){
        Grid newGrid = grid;
        newGrid= makeMove(newGrid,possibleMoves[i].x,possibleMoves[i].y,possibleMoves[i].nextX,possibleMoves[i].nextY,player);

       int moveVal=algo ==1 ? minimax(newGrid,depth,false):alphaBeta(newGrid,depth,false,MIN,MAX);;
            //    cout<<moveVal<<endl;
                if (moveVal > bestVal)
				{
					bestMove = possibleMoves[i];
					bestVal = moveVal;
				}
        }
      //  cout<<"best Vall"<<bestVal<<endl;
        return bestMove;

}

//two players option
void twoPlayers(Grid grid){
    Player player =player1;
    while(!isFinished(grid)){
        int x,y,nextX,nextY;
        printScores();
        printGrid(grid);
        cout<<"Enter the Coords of the cell you wanna move From"<<endl;
        cin>>x>>y;
        cout<<"Enter the Coords of the cell you wanna move To"<<endl;
        cin>>nextX>>nextY;
        grid = makeMove(grid,x,y,nextX,nextY,player);
        cout<<evaluate(grid)<<endl;
        player= player.id == player1.id ? player2 : player1;
        cout<<"---------------------"<<endl;
    }
    printGrid(grid);
    printScores();
    player1.score>player2.score ? cout<<"Player One Has Won" : cout<<"Player Two Has Won";

}
//playing against min max option
void AIplayer(Grid grid,int depth,int algo){
    while(!isFinished(grid)){
        int x,y,nextX,nextY;

        printScores();
        printGrid(grid);

        cout<<"Enter the Coords of the cell you wanna move From"<<endl;
        cin>>x>>y;
        cout<<"Enter the Coords of the cell you wanna move To"<<endl;
        cin>>nextX>>nextY;
        grid = makeMove(grid,x,y,nextX,nextY,player1);
        cout<<evaluate(grid)<<endl;
        const clock_t begin_time = clock();
        Move oneMove = findBestMove(grid,depth,player2,algo);
        std::cout << float( clock () - begin_time ) /  CLOCKS_PER_SEC<<endl;
        cout<<oneMove.x<<" , "<<oneMove.y<<":"<<oneMove.nextX<<" , "<<oneMove.nextY<<endl;
        if(getPossibleMoves(grid,player2).size()!=0)
        grid=makeMove(grid,oneMove.x,oneMove.y,oneMove.nextX,oneMove.nextY,player2);

        cout<<"---------------------"<<endl;
    }
    printGrid(grid);
    printScores();
    player1.score>player2.score ? cout<<"Player One Has Won" : cout<<"Player Two Has Won";

}

//here we run the game
void Game(){
    int rows,cols,option,depth;
    initializePlayers();
    cout<<"Choose your Board Dimensions:"<<endl;
    cin>>rows>>cols;
    Grid grid=makeGrid(rows,cols);
    printGrid(grid);
    cout<<"choose a way to play"<<endl;
    cout<<"1-two players\n2-AI minmax \n3-AI alphabeta"<<endl;
    cin>>option;
    cout<<"how difficult the AI : \n1-easy\n2-a little harder\n3-hard"<<endl;
    cin>>depth;

    switch(option) {
        case 1:
            twoPlayers(grid);
        break;
        case 2:
            AIplayer(grid,depth,1);
        break;
        case 3:
            AIplayer(grid,depth,2);
        break;
        default:
            cout<<"enter a valid choice"<<endl;
    }

}


int main()
{
    Game();
    return 0;
}
