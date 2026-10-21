/*
 * A203185  Number of n X 2 0..4 arrays with every nonzero element less than
 *          or equal to some horizontal or vertical neighbor.
 * https://oeis.org/A203185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A203185: number of n×2 arrays of digits 0..4 in which every
   non‑zero entry has a horizontal or vertical neighbour
   with value ≥ it.  The DP below builds the arrays row by
   row, keeping a 2‑bit mask for each row that records
   whether the left (bit 0) and right (bit 1) cells
   are already satisfied by a neighbour in the same row
   or above.  When adding a new row we enforce that
   all unsatisfied cells of the previous row receive
   a satisfying neighbour from below, and we update
   the satisfaction mask of the new row to include the
   vertical neighbour from above.  At the end we
   sum only the states whose mask is 3 (both cells
   satisfied).  The run‑time is O(n·25²).  */

#include <stddef.h>
#include <stdbool.h>

/* Alphabet values for the 25 possible 2‑column rows. */
static const unsigned char pat[25][2] = {
    {0,0},{0,1},{0,2},{0,3},{0,4},
    {1,0},{1,1},{1,2},{1,3},{1,4},
    {2,0},{2,1},{2,2},{2,3},{2,4},
    {3,0},{3,1},{3,2},{3,3},{3,4},
    {4,0},{4,1},{4,2},{4,3},{4,4}
};

/* Horizontal satisfaction mask for each pattern.
   Bit 0 – left cell satisfied by right neighbour;
   Bit 1 – right cell satisfied by left neighbour. */
static const unsigned char hmask[25] = {
    3,1,1,1,1,   /* 0,0  0,1  0,2  0,3  0,4 */
    2,3,1,1,1,   /* 1,0  1,1  1,2  1,3  1,4 */
    2,2,3,1,1,   /* 2,0  2,1  2,2  2,3  2,4 */
    2,2,2,3,1,   /* 3,0  3,1  3,2  3,3  3,4 */
    2,2,2,2,3    /* 4,0  4,1  4,2  4,3  4,4 */
};

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1, behaviour for n=0 is irrelevant */
        return 0;

    /* DP arrays: 25 patterns × 4 possible masks = 100 states. */
    seq_t dp[100]      = {0};
    seq_t dp_next[100] = {0};
    size_t p, q, s, i;

    /* Initialise states for the first row (no above neighbour). */
    for (p = 0; p < 25; ++p) {
        dp[p * 4 + hmask[p]] = 1;
    }

    /* Add rows 2..n */
    for (i = 2; i <= n; ++i) {
        for (s = 0; s < 100; ++s) dp_next[s] = 0;
        for (s = 0; s < 100; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            p = s / 4;            /* previous pattern index */
            unsigned char mask1 = s & 3;  /* satisfaction mask of previous row */

            /* Try every possible next pattern q. */
            for (q = 0; q < 25; ++q) {
                /* Check that all unsatisfied cells of row p receive
                   a satisfactory neighbour from below (row q). */
                if (!(mask1 & 1) && pat[q][0] < pat[p][0]) continue;
                if (!(mask1 & 2) && pat[q][1] < pat[p][1]) continue;

                /* Compute satisfaction mask for the new row q. */
                unsigned char mask_next = hmask[q];
                if (pat[p][0] >= pat[q][0]) mask_next |= 1;
                if (pat[p][1] >= pat[q][1]) mask_next |= 2;

                /* Accumulate. */
                dp_next[q * 4 + mask_next] += val;
            }
        }
        /* Move to next iteration. */
        for (s = 0; s < 100; ++s) dp[s] = dp_next[s];
    }

    /* Sum all states of the last row that are fully satisfied (mask 3). */
    seq_t result = 0;
    for (p = 0; p < 25; ++p) {
        result += dp[p * 4 + 3];
    }
    return result;
}
