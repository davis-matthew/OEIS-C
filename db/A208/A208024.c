/*
 * A208024  Number of nX4 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 1 0 1 vertically.
 * https://oeis.org/A208024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP computation of OEIS A208024
   Number of n×4 0/1 arrays avoiding
   horizontal 000,001 and vertical 000,101.
   The routine enumerates all 4‑bit rows that satisfy the
   horizontal constraints and then uses dynamic programming
   over triples of rows to enforce the vertical constraint. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* There are exactly 10 allowed rows for a single 4‑bit line:
       0100 (4), 0101 (5), 0110 (6), 0111 (7),
       1010 (10),1011 (11),1100 (12),1101 (13),1110 (14),1111 (15) */
    #define ROWS 10
    static const unsigned char allowedRows[ROWS] =
        {4, 5, 6, 7, 10, 11, 12, 13, 14, 15};

    /* Transition table: trans[a][b][c] == true if rows a,b,c are
       compatible with vertical constraint 000 and 101 (at each column). */
    static bool trans[ROWS][ROWS][ROWS];
    static bool initialized = false;

    if (!initialized) {
        for (int a = 0; a < ROWS; ++a)
            for (int b = 0; b < ROWS; ++b)
                for (int c = 0; c < ROWS; ++c) {
                    bool ok = true;
                    for (int col = 0; col < 4; ++col) {
                        int ma = (allowedRows[a] >> col) & 1;
                        int mb = (allowedRows[b] >> col) & 1;
                        int mc = (allowedRows[c] >> col) & 1;
                        int triple = (ma << 2) | (mb << 1) | mc;
                        if (triple == 0 || triple == 5) { /* 000 or 101 */
                            ok = false;
                            break;
                        }
                    }
                    trans[a][b][c] = ok;
                }
        initialized = true;
    }

    if (n == 0) return (seq_t)0;   /* outside the offset, value irrelevant */
    if (n == 1) return (seq_t)ROWS;
    if (n == 2) return (seq_t)ROWS * (seq_t)ROWS; /* all pairs allowed */

    /* dpPrev[i][j] = number of ways to end with rows i (n-2) and j (n-1) */
    seq_t dpPrev[ROWS][ROWS];
    seq_t dpNext[ROWS][ROWS];

    /* initialisation for rows 1 and 2: all pairs allowed */
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < ROWS; ++j)
            dpPrev[i][j] = (seq_t)1;

    /* iterate from row 3 up to row n */
    for (idx_t row = 3; row <= n; ++row) {
        /* zero dpNext */
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                dpNext[i][j] = (seq_t)0;

        /* transition */
        for (int a = 0; a < ROWS; ++a) {
            for (int b = 0; b < ROWS; ++b) {
                seq_t val = dpPrev[a][b];
                if (!val) continue;
                for (int c = 0; c < ROWS; ++c) {
                    if (trans[a][b][c]) {
                        dpNext[b][c] += val;
                    }
                }
            }
        }

        /* move dpNext -> dpPrev for next iteration */
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                dpPrev[i][j] = dpNext[i][j];
    }

    /* sum over all possible last two rows */
    seq_t result = (seq_t)0;
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < ROWS; ++j)
            result += dpPrev[i][j];

    return result;
}
