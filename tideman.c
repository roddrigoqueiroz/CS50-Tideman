#include <cs50.h>
#include <stdio.h>
#include <string.h>

/*The tideman voting system, tells the winner by looking at every pair of candidates and
determining who would have won the ellection if it was only btw those 2 candidates inside the pair*/

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner; // records the winner index
    int winners_votes; // This line does not belong to the source code. I added it to sort the pairs array
    int loser; // records the loser index
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool find_cicle(int a, int *counter); // Compares the loser in ith position with all the past winners
int find_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // This function works as it should
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// THIS FUNCTION WORKS!!

// O erro desta função está quando atribuo o preferences[i][j] = 0, pois, dependendo do valor de i e j,
// a função vai zerar um valor que não deveria. Assim, se
/*  Rank 1: Alice
    Rank 2: Bob
    Rank 3: Charlie

    Rank 1: Charlie
    Rank 2: Alice
    Rank 3: Bob

    Rank 1: Bob
    Rank 2: Charlie
    Rank 3: Alice
   No terceiro voto, Bob deveria ter 2 votos sobre Charlie (voto 1 + voto 3), porém, pela forma
   com que o código está escrito, o voto 2 anula o primeiro voto de Bob sobre Charlie (preferences[1][2] = 0)
   e perco o valor que eu havia atualizado.
   */

// JÁ ARRUMEI, MAS FICA DE ALERTA

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // This function works as it should
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // The i has to be grater than j, because i indicates the candidates rank.
            // So as much lower i is, the grater that candidate is in the voters rank.
            if (i < j)
                preferences[ranks[i]][ranks[j]]++; //prefereces records the votes of any voter and candidate
            /*printf("Number of votes: %d\n", preferences[ranks[i]][ranks[j]]);
            printf("Candidate i index: %d\n", ranks[i]);
            printf("Candidate j index: %d\n", ranks[j]);
            matriz [i][j]*/
        }
    }
    return;
}

// THIS FUNCTION WORKS!!

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // This 2 for loops are used to go through the preferences array
    // Maybe I can use, in this function, another array to save the number of votes of the winner
    // and than I just sort this array.
    // If I could have in the same pair strcut a .winners_votes it wold be a lot easier to sort the pais array.
    // -> Check if I can modify this struct (I think it gets better coded)
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j ++)
        {
            // Here I compare the candidate i with j and candidate j with i only once
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pairs[pair_count].winners_votes = preferences[i][j]; // Using this class, I can sort easily
                // printf("Ganhador: %i\n", pairs[pair_count].winner);
                // printf("Perdedor: %i\n", pairs[pair_count].loser);
                pair_count++;
            }
            /*if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                printf("Ganhador: %i\n", pairs[pair_count].winner);
                printf("Perdedor: %i\n", pairs[pair_count].loser);
                pair_count++;
            }*/
        }
    }
    return;
}

//THIS FUNCTION IS WRONG. FOR SOME REASON, IN THE CS50 TESTS IT DOESN'T SORT FOR ALL PAIRS

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // I have to sort the pairs array by order of strength of victory. It means that a
    // winner with a greater number of votes should comes first.
    int stronger = 0, index = 0, n;
    int winner, loser;

    while (index < pair_count)
    {
        for (int i = index; i < pair_count; i++)
        {
            if (pairs[i].winners_votes > stronger)
            {
                stronger = pairs[i].winners_votes;
                n = i;
                winner = pairs[i].winner;
                loser = pairs[i].loser;
                /*Assim que termino esse for, eu tenho a certeza de que o valor dentro de stronger corresponde
                aos votos do candidato mais votado, assim como eu tenho tb o índice desse candidato (em qual posição
                do array pairs ele está)*/
            }
        }
        //Como eu já tenho o indice do candidato vencedor e perdedor, posso atribuir outro valor para pairs.winner

        // Swaps each value
        if (n != index)
        {
            pairs[n].winner = pairs[index].winner;
            pairs[n].loser = pairs[index].loser;
            pairs[n].winners_votes = pairs[index].winners_votes;
            pairs[index].winner = winner; //Aqui o array na posição ordenada recebe o índice do vencedor
            pairs[index].loser = loser;
            pairs[index].winners_votes = stronger;
        }
        index++;
        stronger = 0;
    }
    for (int i = 0; i < pair_count; i++)
        //printf("pairs[%i].winners_votes = %i\n", i, pairs[i].winners_votes);
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
// Its creating inner cycles (cycles with less than candidate count for more than 3)
void lock_pairs(void) //I dont know if its wrong or right
{
    // TODO
    // If I have all the candidates in both positions of array locked (locked[position1][position2])
    // I'll have a cicle.
    int counter = 0; // This is a flag variable that searchs for a past winner that lose to someone

    // If state = pair_count, it means that I have a cicle
    // I only have a cicle when a loser is locked in with a winner. It means that a cicle is created when a
    // loser recieves the i index and a previous winner recieves a j index.
    //                 i                 j
    // if (pairs[i].winner && pairs[i].loser and the loser was a winner and the winner was a loser)
    //      I'll have a cicle
    for (int i = 0; i < pair_count; i++)
    {
        if (!find_cicle(i, &counter))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
            //printf("Locked: %i, w = %i, l = %i\n", locked[pairs[i].winner][pairs[i].loser], pairs[i].winner, pairs[i].loser);
        }
    }
    printf("False: %i\n", false);
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    printf("%s\n", candidates[find_winner()]);
    return;
}

// I only have a cicle when a pairs[i].winner == pairs[j].loser && pairs[i].loser == pairs[j].winner
bool find_cicle(int a, int *counter)
{
    /* O loop é determinado pela existência de todos os candidatos em todos os índices. Isso significa que
    se todos os candidatos aparecerem em i e j (independente da ordem) tenho um loop.
    Agora é só escrever isso em C
    Basicamente, se locked[i][j] for true para todos os índices de candidatos */
    // At this time this function can only find cicles that use all candidates.
    for (int i = 0; i < a; i++)
    {
        // if (pairs[a].winner == pairs[i].loser && pairs[a].loser == pairs[i].winner) // When a == i, the loser will be != than the winner
        //     return true;
        if (pairs[a].winner == pairs[i].loser)
            *counter+=1;
        if (pairs[a].loser == pairs[i].winner)
            *counter+=1;
    }
    if (*counter >= candidate_count)
        return true;
    else
        return false;
}

int find_winner(void)
{
    /* O ganhador é o candidato que, para todas as posições de locked[i][j] que o envolvam na posição j
    (locked[i][0], para se referir ao candidato a) sejam false. Se isso acontecer, ele será o ganhador e
    devo retornar o índice do candidato. Logo, faz sentido que eu use pair_count e um contador p/ isso. */
    int counter = 0;
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < pair_count; j++)
        {
            if (locked[j][i] == false)
                counter++;
            else
                break;
        }
        if (counter == pair_count)
            return i;
        else
            counter = 0;
    }
    return -1;
}