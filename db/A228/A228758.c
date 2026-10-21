/*
 * A228758  Number of 6Xn binary arrays with top left element equal to 1 and
 *          no two ones adjacent horizontally or antidiagonally.
 * https://oeis.org/A228758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A228758: Number of 6×n binary arrays with top‑left element 1
   and no two ones adjacent horizontally or anti‑diagonally.
   Computed by a dynamic‑programming recurrence on column patterns.
   The recurrence: let dp[s] be the number of ways to fill all processed
   columns whose last column has pattern s (a 6‑bit mask).  For the first
   column we allow all patterns with bit 0 = 1.  For each next column c,
   we may append it to a previous pattern p iff
     (p & c) == 0                 // horizontal adjacency forbidden
     && ((p >> 1) & c) == 0       // anti‑diagonal adjacency forbidden
   The number of full arrays of width n is the sum of dp[s] after n steps. */

#include <stddef.h>
#include <stdint.h>

#define STATES (1 << 6)   /* 64 possible 6‑bit column patterns */

seq_t a(idx_t n)
{
    /* Transition matrix: allowed[p][c] == 1 iff column pattern c can
       follow pattern p.  Only filled once. */
    static unsigned char allowed[STATES][STATES];
    static int init_done = 0;

    if (!init_done) {
        for (int p = 0; p < STATES; ++p) {
            for (int c = 0; c < STATES; ++c) {
                if (p & c) {                /* same row has 1 in both columns */
                    allowed[p][c] = 0;
                } else if ((p >> 1) & c) {   /* anti‑diagonal conflict */
                    allowed[p][c] = 0;
                } else {
                    allowed[p][c] = 1;
                }
            }
        }
        init_done = 1;
    }

    /* DP arrays for the current and next column. */
    seq_t dp[STATES];
    seq_t dp_next[STATES];

    /* Initialisation: first column must have bit 0 = 1. */
    for (int s = 0; s < STATES; ++s) {
        dp[s] = (s & 1) ? (seq_t)1 : (seq_t)0;
    }

    seq_t result = 0;          /* will hold the value for the requested n */

    /* Process columns 1 .. n. */
    for (idx_t col = 1; col <= n; ++col) {
        seq_t sum = 0;

        /* Sum over all patterns for this column. */
        for (int s = 0; s < STATES; ++s) sum += dp[s];

        /* If this is the requested column, we are done. */
        if (col == n) {
            result = sum;
            break;
        }

        /* Compute dp_next for the next column. */
        for (int s = 0; s < STATES; ++s) dp_next[s] = 0;

        for (int p = 0; p < STATES; ++p) {
            seq_t val = dp[p];
            if (!val) continue;
            for (int c = 0; c < STATES; ++c) {
                if (allowed[p][c]) {
                    dp_next[c] += val;
                }
            }
        }

        /* Advance to next column. */
        for (int s = 0; s < STATES; ++s) dp[s] = dp_next[s];
    }

    return result;
}
