/*
 * A061221  Number of tic-tac-toe games won after n plays.
 * https://oeis.org/A061221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = number of distinct tic‑tac‑toe games that finish with the
 *            first win on the nth move. Computed by exhaustive depth‑first
 *            search of the game tree (board positions 0–8). Only n ≥ 1 are
 *            meaningful; a(n) = 0 for n < 1 or n > 9
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Winning triples (rows, columns, diagonals). */
static const unsigned char WIN_LINES[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
    {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
    {0, 4, 8}, {2, 4, 6}
};

/* Cache of results up to n = 9.  Index 0 unused. */
static seq_t COUNTS[10] = {0};
static bool DONE = false;

/* Test whether player p (1=X, 2=O) has a winning line. */
static bool has_won(const unsigned char board[9], unsigned char p)
{
    for (int i = 0; i < 8; ++i) {
        const unsigned char *l = WIN_LINES[i];
        if (board[l[0]] == p && board[l[1]] == p && board[l[2]] == p)
            return true;
    }
    return false;
}

/* Recursive depth‑first search.  'player' is the last player who moved. */
static void dfs(unsigned char board[9], unsigned char player, idx_t m)
{
    /* Check if the last move produced a win. */
    if (has_won(board, player)) {
        COUNTS[(idx_t)m] += (seq_t)1;
        return;
    }

    /* If the board is full, it's a draw. */
    if (m == 9) return;

    /* Next player to move. */
    unsigned char next = (player == 1 ? 2 : 1);

    /* Explore each empty cell. */
    for (int pos = 0; pos < 9; ++pos) {
        if (board[pos] == 0) {
            board[pos] = next;
            dfs(board, next, m + 1);
            board[pos] = 0;  /* backtrack */
        }
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    /* Compute once. */
    if (!DONE) {
        unsigned char board[9] = {0};

        /* First move always by player 1 (X). */
        for (int pos = 0; pos < 9; ++pos) {
            board[pos] = 1;
            dfs(board, 1, 1);  /* last move was by X */
            board[pos] = 0;
        }
        DONE = true;
    }

    if (n < 1 || n > 9) return (seq_t)0;
    return COUNTS[(idx_t)n];
}
