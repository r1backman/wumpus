#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int wumpus, bat1, bat2, pit1, pit2, player, arrow_quiver = 5, shot_room;
int cave_doors[20][3] = {
    {1,4,7}, {0,2,9}, {1,3,11}, {2,4,13}, {0,3,5}, {4,6,14}, {5,7,19},
    {0,6,8}, {7,9,18}, {1,8,10}, {9,11,17}, {2,10,12}, {11,13,16},
    {3,12,14}, {5,13,15}, {14,16,19}, {12,15,17}, {10,16,18}, {8,17,19}, {6,15,18}
};

void init_all_values()
{
    srand((unsigned)time(NULL));
    wumpus = rand() % 20;
    bat1 = rand() % 20;
    bat2 = rand() % 20;
    pit1 = rand() % 20;
    pit2 = rand() % 20;
    player = rand() % 20;
    while  (wumpus == bat1 || wumpus == bat2 || wumpus == pit1 || wumpus == pit2 || wumpus == player) {
        wumpus = rand() % 20;
    }
    while  (bat1 == wumpus || bat1 == bat2 || bat1 == pit1 || bat1 == pit2 || bat1 == player) {
        bat1 = rand() % 20;
    }
    while  (bat2 == wumpus || bat2 == bat1 || bat2 == pit1 || bat2 == pit2 || bat2 == player) {
        bat2 = rand() % 20;
    }
    while  (pit1 == wumpus || pit1 == bat1 || pit1 == bat2 || pit1 == pit2 || pit1 == player) {
        pit1 = rand() % 20;
    }
    while  (pit2 == wumpus || pit2 == bat1 || pit2 == bat2 || pit2 == pit1 || pit2 == player) {
        pit2 = rand() % 20;
    }
    while  (player == wumpus || player == bat1 || player == bat2 || player == pit1 || player == pit2) {
        player = rand() % 20;
    }
    // cheating
    // printf("wumpus>%i bat1>%i bat2>%i pit1>%i pit2>%i player>%i\n", wumpus+1, bat1+1, bat2+1, pit1+1, pit2+1, player+1);    
}

void display_heading()
{
    
    printf("HUNT THE WUMPUS\n");
    printf("===============\n\n");
}

void select_room()
{
    char *end;
    long number;
    char in_str[50];
    int good = 1;
    
    do {
        printf("Where to? ");
        scanf("%s", in_str);
        number = strtol(in_str, &end, 0);
        if (*end != '\0' ||
            (number != cave_doors[player][0]+1 &&
             number != cave_doors[player][1]+1 &&
             number != cave_doors[player][2]+1)
        ) {
            printf("You cannot walk that far. Please use one of the tunnels.\n");
            good = 0;
        }
        else {
            good = 1;
        }
        fflush(stdin);
    } while (good == 0);
    player = number - 1;
}

int shoot_at_room()
{
    char *end;
    long number;
    char in_str[50];
    
    printf("Where to? ");
    scanf("%s", in_str);
    number = strtol(in_str, &end, 0);
    shot_room = number;
    printf("Shooting an arrow into room %i...\n", shot_room);
    arrow_quiver--;
    fflush(stdin);
    if (*end != '\0' ||
        (shot_room != cave_doors[player][0]+1 &&
            shot_room != cave_doors[player][1]+1 &&
            shot_room != cave_doors[player][2]+1 
        )
    ) {
        printf("This arrow is lost.\n");
        return 0;
    }
    else {
        shot_room--;
        return 1;
    }
}

void get_new_random_room(int current_player)
{
    player = rand() % 20;
    while  (player == wumpus || player == bat1 || player == bat2 || player == pit1 || player == pit2 || player == current_player) {
        player = rand() % 20;
    }
}

int check_unoccupied (int currrent_location)
{
    int count = 0, index, j;
    char room[3];
    
    room[0] = room[1] = room[2] = 'n';
    
    if (cave_doors[currrent_location][0] != bat1 &&
         cave_doors[currrent_location][0] != bat2 &&
         cave_doors[currrent_location][0] != pit1 &&
         cave_doors[currrent_location][0] != pit2) {
        room[0] = 'y';
        count++;
    }
    if (cave_doors[currrent_location][0] != bat1 &&
         cave_doors[currrent_location][0] != bat2 &&
         cave_doors[currrent_location][0] != pit1 &&
         cave_doors[currrent_location][0] != pit2) {
        room[1] = 'y';
        count++;
    }
    if (cave_doors[currrent_location][0] != bat1 &&
         cave_doors[currrent_location][0] != bat2 &&
         cave_doors[currrent_location][0] != pit1 &&
         cave_doors[currrent_location][0] != pit2) {
        room[2] = 'y';
        count++;
    }
    if (count == 0) {
        return 200;
    }
    switch (count) {
        case 3 :
            index = rand() % 3;
            return cave_doors[currrent_location][index];
            break;
        case 2 :
            index = rand() % 2;
            for (j=0; j < 3; j++) {
                if (room[j] == 'y' && index == 0) {
                    return cave_doors[currrent_location][j];
                }
            }
            for (j=2; j >= 0; j--) {
                if (room[j] == 'y' && index == 1) {
                    return cave_doors[currrent_location][j];
                }
            }
            break;
    }
    for (j=0; j < 3; j++) {
        if (room[j] == 'y') {
            return cave_doors[currrent_location][j];
        }
    }
    return 200;
}

int check_game_state(int shooting)
{
    int wumpus_move, wumpus_new_room, wumpus_moved = 0;
    
    if (shooting == 0) {
        printf("Entering room %i...\n", player + 1);
    }
    else {
        if (arrow_quiver < 0) {
            printf("Your quiver is empty.\n\nGame over!\n");
            return 0;
        }
        wumpus_move = rand() % 4;
        if (wumpus_move == 0 || wumpus_move == 1 || wumpus_move == 2) {
            wumpus_new_room = check_unoccupied(wumpus);
            if (wumpus_new_room != 200) {
                wumpus = wumpus_new_room;
                // cheating
                // printf("wumpus>%i bat1>%i bat2>%i pit1>%i pit2>%i player>%i\n", wumpus+1, bat1+1, bat2+1, pit1+1, pit2+1, player+1);
            }
        }
        if (shot_room == wumpus) {
            printf("Hurra, you killed the wumpus!\n\nGame over!\n");
            return 0;
        }
    }
    if (player == wumpus) {
        printf("Wumpus eats you.\n\nGame over!\n");
        return 0;
    }
    if (player == pit1 || player == pit2) {
        printf("You fall into a pit.\n\nGame over!\n");
        return 0;
    }
    if (player == bat1 || player == bat2) {
        printf("You encounter a bat, it transports you to a random empty room.\n");
        get_new_random_room(player);
        printf("Entering room %i...\n", player + 1);
    }
    if (wumpus == cave_doors[player][0] ||
        wumpus == cave_doors[player][1] ||
        wumpus == cave_doors[player][2]
    ) {
        printf("You smell something terrible nearby.\n");
    }
    if (bat1 == cave_doors[player][0] ||
        bat1 == cave_doors[player][1] ||
        bat1 == cave_doors[player][2] ||
        bat2 == cave_doors[player][0] ||
        bat2 == cave_doors[player][1] ||
        bat2 == cave_doors[player][2]
    ) {
        printf("You hear a rustling.\n");
    }
    if (pit1 == cave_doors[player][0] ||
        pit1 == cave_doors[player][1] ||
        pit1 == cave_doors[player][2] ||
        pit2 == cave_doors[player][0] ||
        pit2 == cave_doors[player][1] ||
        pit2 == cave_doors[player][2]
    ) {
        printf("You feel a cold wind blowing from a nearby cavern.\n");
    }
    printf("You are in room %i. Tunnels lead to:  %i  %i  %i\n",
        player +1, cave_doors[player][0]+1, 
        cave_doors[player][1]+1, cave_doors[player][2]+1
    );
    return 1;
}

void game_loop()
{
    char line[50];

    if (check_game_state(0) == 0) {
        return;
    }
    while(1) {
        printf("Shoot or move (S-M)? ");
        scanf("%s", line);
        line[0] = toupper(line[0]);
        if (strlen(line) > 1) {
            line[0] = 'z';
        }

        switch (line[0]) {
            case 'M' :
                select_room();
                if (check_game_state(0) == 0) {
                    return;
                }
                else {
                    break;
                }
            case 'S' :
                shoot_at_room();
                if (check_game_state(1) == 0) {
                    return;
                }
                else {
                    break;
                }
            case 'Q' :
                printf("quitting...\n");
                return;
            default:
                printf("This is not a valid action: pick 'S' to shoot and 'M' to move.\n");
        }
        fflush(stdin);
    }
}

int main(int argc, char *argv[])
{
    init_all_values();
    display_heading();
    game_loop();
    return 0;
}
