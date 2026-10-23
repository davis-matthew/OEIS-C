/*
 * A186818  Half the number of (n+2)X4 binary arrays with each 3X3 subblock
 *          having sum 4 or 5.
 * https://oeis.org/A186818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements A186818:  half the number of (n+2)×4 binary arrays
   with each 3×3 subblock having sum 4 or 5.  The value is computed
   by dynamic programming over rows, counting all valid arrays of
   height H = n+2 and then dividing by 2. */
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Number of rows in the array */
    idx_t H = n + 2;
    if (H < 3) return 0;          /* not defined for n < 1, but fall‑back */

    /* Pre‑compute the truth table for triples of rows that satisfy
       the 3×3 subblock condition.  Each row is a 4‑bit vector 0..15. */
    static bool sat[16][16][16];
    static bool init_sat = false;
    if (!init_sat) {
        init_sat = true;
        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b)
                for (int c = 0; c < 16; ++c) {
                    int sum0 = 0; /* 3×3 block cols 0..2 */
                    int sum1 = 0; /* 3×3 block cols 1..3 */
                    for (int r = 0; r < 3; ++r) {
                        int row = (r == 0 ? a : r == 1 ? b : c);
                        /* bits 0..2 */
                        sum0 += (row >> 0) & 1;
                        sum0 += (row >> 1) & 1;
                        sum0 += (row >> 2) & 1;
                        /* bits 1..3 */
                        sum1 += (row >> 1) & 1;
                        sum1 += (row >> 2) & 1;
                        sum1 += (row >> 3) & 1;
                    }
                    sat[a][b][c] = (sum0 == 4 || sum0 == 5) &&
                                    (sum1 == 4 || sum1 == 5);
                }
    }

    /* Dynamic programming tables for the last two rows */
    seq_t dp[16][16];
    seq_t ndp[16][16];

    /* All pairs of first two rows are admissible (no 3x3 subblock yet). */
    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            dp[a][b] = 1;

    /* Add rows from index 2 up to H-1 (total H-2 new rows). */
    for (idx_t row = 2; row < H; ++row) {
        /* zero the next table */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b) {
                seq_t val = dp[a][b];
                if (val == 0) continue;
                for (int c = 0; c < 16; ++c)
                    if (sat[a][b][c])
                        ndp[b][c] += val;
            }

        /* copy ndp back into dp for the next iteration */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Sum all counts for the final two rows */
    seq_t total = 0;
    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            total += dp[a][b];

    /* The required sequence is half the total number of arrays. */
    return total / 2;
}
