/*
 * A237532  Number of (n+1) X (1+1) 0..3 arrays with the upper median minus
 *          the lower median of every 2 X 2 subblock equal.
 * https://oeis.org/A237532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) equals the number of (n+1)×2 arrays with entries in
 *            {0,1,2,3} such that * the difference between the upper and lower
 *            median of every 2×2 sub‑block is * the same for all sub‑blocks.
 *            * Implementation by dynamic programming over the 16 possible row
 *            states
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)                 /* sequence is defined only for n ≥ 1 */
        return 0;

    /* transition[d][i][j] == 1 iff the transition from state i to j
     * yields difference d = (upper median) – (lower median).  */
    static unsigned char transition[4][16][16];
    static int init_flag = 0;

    if (!init_flag) {          /* initialize once */
        for (int d = 0; d < 4; ++d)
            for (int i = 0; i < 16; ++i)
                for (int j = 0; j < 16; ++j)
                    transition[d][i][j] = 0;

        for (int i = 0; i < 16; ++i) {
            int a1 = i / 4, b1 = i % 4;
            for (int j = 0; j < 16; ++j) {
                int a2 = j / 4, b2 = j % 4;
                int t[4] = { a1, b1, a2, b2 };

                /* insert‑sort the four values */
                for (int p = 1; p < 4; ++p) {
                    int key = t[p], k = p - 1;
                    while (k >= 0 && t[k] > key) {
                        t[k + 1] = t[k];
                        --k;
                    }
                    t[k + 1] = key;
                }
                int diff = t[2] - t[1];      /* upper median – lower median */
                transition[diff][i][j] = 1;
            }
        }
        init_flag = 1;
    }

    size_t steps = (size_t)n;          /* number of 2×2 sub‑blocks          */
    seq_t total = 0;

    /* Count sequences for each possible constant difference d */
    for (int d = 0; d < 4; ++d) {
        seq_t dp[16], next[16];

        /* Any first row is allowed */
        for (int i = 0; i < 16; ++i)
            dp[i] = 1;

        for (size_t s = 0; s < steps; ++s) {
            for (int i = 0; i < 16; ++i)
                next[i] = 0;

            for (int i = 0; i < 16; ++i) if (dp[i]) {
                for (int j = 0; j < 16; ++j) if (transition[d][i][j]) {
                    next[j] = next[j] + dp[i];
                }
            }

            for (int i = 0; i < 16; ++i)
                dp[i] = next[i];
        }

        /* Sum over all end states */
        seq_t count = 0;
        for (int i = 0; i < 16; ++i)
            count += dp[i];

        total += count;
    }
    return total;
}
