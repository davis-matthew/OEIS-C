/*
 * A048245  Number of distinct terminal tic-tac-toe positions after n plays,
 *          up to rotation and reflection.
 * https://oeis.org/A048245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   Number of distinct terminal tic‑tac‑toe positions after n plays,
 *            up to rotation and reflection. The value is pre‑computed from
 *            the full state space (3^9 states)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Winning lines */
static const int lines[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
    {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
    {0, 4, 8}, {2, 4, 6}
};

/* 8 symmetries: new index -> old index */
static const int sym[8][9] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8},          /* identity */
    {6, 3, 0, 7, 4, 1, 8, 5, 2},          /* rotate 90° */
    {8, 7, 6, 5, 4, 3, 2, 1, 0},          /* rotate 180° */
    {2, 5, 8, 1, 4, 7, 0, 3, 6},          /* rotate 270° */
    {2, 1, 0, 5, 4, 3, 8, 7, 6},          /* mirror vertical */
    {6, 7, 8, 3, 4, 5, 0, 1, 2},          /* mirror horizontal */
    {0, 3, 6, 1, 4, 7, 2, 5, 8},          /* mirror main diagonal */
    {8, 5, 2, 7, 4, 1, 6, 3, 0}           /* mirror anti‑diagonal */
};

/* Cache for a(n) values, indexed by n (0 ≤ n ≤ 9) */
static seq_t cache[10];
static bool   initialized = false;

/* Test whether board has a winner (player 1 = X, 2 = O) */
static bool board_has_win(const int *b, int player)
{
    for (int w = 0; w < 8; ++w) {
        int a = lines[w][0], c = lines[w][1], d = lines[w][2];
        if (b[a] == player && b[c] == player && b[d] == player)
            return true;
    }
    return false;
}

/* Compute all terminal boards and populate cache */
static void init_cache(void)
{
    if (initialized)
        return;

    /* Canonical representatives for each n */
    static seq_t canon[10][512];
    static unsigned canon_cnt[10];
    for (int i = 0; i < 10; ++i) {
        cache[i] = 0;
        canon_cnt[i] = 0;
    }

    const int TOTAL = 19683; /* 3^9 */
    int board[9];

    for (int id = 0; id < TOTAL; ++id) {
        /* Decode board from base‑3 representation */
        int tmp = id;
        for (int i = 0; i < 9; ++i) {
            board[i] = tmp % 3;  /* 0 empty, 1 X, 2 O */
            tmp /= 3;
        }

        int countX = 0, countO = 0;
        for (int i = 0; i < 9; ++i) {
            if (board[i] == 1) ++countX;
            else if (board[i] == 2) ++countO;
        }
        int n = countX + countO;
        if (n == 0 || n > 9)
            continue;

        /* Parity consistency */
        if (n & 1) {
            if (countX != countO + 1) continue;
        } else {
            if (countX != countO) continue;
        }

        /* Winning check */
        bool winX = board_has_win(board, 1);
        bool winO = board_has_win(board, 2);
        if (winX && winO)
            continue;
        if (winX && !(n & 1))
            continue;
        if (winO && (n & 1))
            continue;

        bool legal = false;
        if (winX || winO) {
            int lastPlayer = winX ? 1 : 2;
            for (int i = 0; i < 9 && !legal; ++i) {
                if (board[i] != lastPlayer)
                    continue;

                int b2[9];
                for (int k = 0; k < 9; ++k) b2[k] = board[k];
                b2[i] = 0;  /* remove last move */

                if (board_has_win(b2, 1) || board_has_win(b2, 2))
                    continue;  /* earlier win existed */

                int cX = countX - (lastPlayer == 1);
                int cO = countO - (lastPlayer == 2);
                if ((n - 1) & 1) {
                    if (cX != cO + 1) continue;
                } else {
                    if (cX != cO) continue;
                }
                legal = true;
            }
        } else { /* no winner – must be full board */
            if (n == 9)
                legal = true;
        }
        if (!legal)
            continue;

        /* Canonical representation under symmetry */
        seq_t minRep = 0;
        for (int s = 0; s < 8; ++s) {
            seq_t rep = 0;
            for (int j = 0; j < 9; ++j) {
                int val = board[sym[s][j]];
                rep = rep * (seq_t)3 + (seq_t)val;
            }
            if (s == 0 || rep < minRep)
                minRep = rep;
        }

        unsigned cnt = canon_cnt[n];
        bool exists = false;
        for (unsigned k = 0; k < cnt; ++k) {
            if (canon[n][k] == minRep) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            canon[n][cnt] = minRep;
            canon_cnt[n] = cnt + 1;
        }
    }

    for (int i = 0; i < 10; ++i)
        cache[i] = (seq_t)canon_cnt[i];

    initialized = true;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* For n > 9 the game cannot have more than 9 plays */
    if (n >= 10)
        return (seq_t)0;
    init_cache();
    return cache[(int)n];
}
