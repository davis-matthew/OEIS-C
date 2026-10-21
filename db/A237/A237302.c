/*
 * A237302  Number of (n+1)X(2+1) 0..2 arrays with the upper median of every
 *          2X2 subblock equal.
 * https://oeis.org/A237302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with entries {0,1,2} such that
 *            the upper median of every 2×2 subblock is equal
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute allowed row transitions for each possible upper median m (0..2). */
    static bool initialized = false;
    static bool allowed[3][27][27];
    if (!initialized) {
        for (int m = 0; m < 3; ++m) {
            for (int p = 0; p < 27; ++p) {
                int a0 = p / 9;          /* first row: (a0,b0,c0) */
                int b0 = (p / 3) % 3;
                int c0 = p % 3;
                for (int q = 0; q < 27; ++q) {
                    int a1 = q / 9;          /* second row: (a1,b1,c1) */
                    int b1 = (q / 3) % 3;
                    int c1 = q % 3;

                    bool ok = true;

                    /* Check the two 2×2 subblocks formed with these rows. */
                    int v[4];
                    /* Subblock j = 0: a0,b0,a1,b1 */
                    v[0] = a0; v[1] = b0; v[2] = a1; v[3] = b1;
                    for (int i = 0; i < 4; ++i)
                        for (int j = i + 1; j < 4; ++j)
                            if (v[i] > v[j]) { int t = v[i]; v[i] = v[j]; v[j] = t; }
                    if (v[2] != m) ok = false;

                    if (ok) {
                        /* Subblock j = 1: b0,c0,b1,c1 */
                        v[0] = b0; v[1] = c0; v[2] = b1; v[3] = c1;
                        for (int i = 0; i < 4; ++i)
                            for (int j = i + 1; j < 4; ++j)
                                if (v[i] > v[j]) { int t = v[i]; v[i] = v[j]; v[j] = t; }
                        if (v[2] != m) ok = false;
                    }

                    allowed[m][p][q] = ok;
                }
            }
        }
        initialized = true;
    }

    /* DP arrays: dp_cur[m][state] holds number of arrays up to current row
       with the specified upper median m and ending in the given row state. */
    static seq_t dp_cur[3][27];
    static seq_t dp_next[3][27];

    /* Start with a single arbitrary first row. */
    for (int m = 0; m < 3; ++m)
        for (int s = 0; s < 27; ++s)
            dp_cur[m][s] = 1;

    /* Perform n steps (rows = n+1). */
    for (idx_t step = 0; step < n; ++step) {
        /* Clear next DP array. */
        for (int m = 0; m < 3; ++m)
            for (int q = 0; q < 27; ++q)
                dp_next[m][q] = 0;

        /* Transition to the next row. */
        for (int m = 0; m < 3; ++m) {
            for (int p = 0; p < 27; ++p) {
                seq_t val = dp_cur[m][p];
                if (val == 0) continue;
                for (int q = 0; q < 27; ++q) {
                    if (allowed[m][p][q])
                        dp_next[m][q] += val;
                }
            }
        }

        /* Copy next state into current for the following iteration. */
        for (int m = 0; m < 3; ++m)
            for (int s = 0; s < 27; ++s)
                dp_cur[m][s] = dp_next[m][s];
    }

    /* Sum counts over all medians and ending states. */
    seq_t total = 0;
    for (int m = 0; m < 3; ++m)
        for (int s = 0; s < 27; ++s)
            total += dp_cur[m][s];

    return total;
}
