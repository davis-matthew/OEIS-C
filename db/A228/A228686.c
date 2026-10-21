/*
 * A228686  Number of 5 X n binary arrays with no two ones adjacent
 *          horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Number of 5×n binary arrays with no two adjacent ones horizontally, 
 * diagonally, or antidiagonally.  Computed by a transfer‑matrix dynamic 
 * programming algorithm.  The recurrence given in the OEIS entry is NOT 
 * used directly; instead we build all 32 column states (5‑bit patterns) and 
 * count compatible ways to extend each column.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not part of the sequence (offset = 1) */

    /* Precompute compatibility of the 32 possible column patterns */
    static unsigned char compat[32][32];
    static int compat_done = 0;
    if (!compat_done) {
        for (int s = 0; s < 32; ++s) {
            for (int t = 0; t < 32; ++t) {
                unsigned int ss = s, tt = t;
                unsigned int inter   = ss & tt;
                unsigned int diag1   = ss & (tt << 1);   /* diagonal (i, j) with j = i+1 */
                unsigned int diag2   = ss & (tt >> 1);   /* antidiagonal (i, j) with j = i-1 */
                compat[s][t] = (inter == 0 && diag1 == 0 && diag2 == 0);
            }
        }
        compat_done = 1;
    }

    /* Dynamic programming over the columns */
    seq_t dp_prev[32];
    seq_t dp_next[32];

    /* First column: any of the 32 states is allowed (value 1 each) */
    for (int s = 0; s < 32; ++s) dp_prev[s] = 1;

    /* Extend to columns 2 … n */
    for (idx_t col = 2; col <= n; ++col) {
        /* clear dp_next */
        for (int t = 0; t < 32; ++t) dp_next[t] = 0;

        /* For each previous state s add its ways to all compatible next states t */
        for (int s = 0; s < 32; ++s) {
            seq_t val = dp_prev[s];
            if (val == 0) continue;
            for (int t = 0; t < 32; ++t) {
                if (compat[s][t]) dp_next[t] += val;
            }
        }

        /* swap dp_prev and dp_next for next iteration */
        for (int t = 0; t < 32; ++t) dp_prev[t] = dp_next[t];
    }

    /* Sum over all possible last column states */
    seq_t total = 0;
    for (int t = 0; t < 32; ++t) total += dp_prev[t];
    return total;
}
