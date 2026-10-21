/*
 * A079922  Solution to the Dancing School Problem with n girls and n+3 boys:
 *          f(n,3).
 * https://oeis.org/A079922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permanent of the n × (n+3) 0/1-matrix B with * B[i][j] =
 *            1 iff i ≤ j ≤ i+3 (indices start at 1). * This permanent counts
 *            the matchings of size n in a bipartite graph * where each row i
 *            can be matched to any of the next four columns. * Computed by a
 *            DP over a sliding window of width 4
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* n is in the range ≥1; for n==0 return 0 */
    if (n == 0) return (seq_t)0;

    /* DP arrays for 16 possible masks (4-bit) */
    seq_t dp[16] = {0};
    seq_t ndp[16] = {0};

    dp[0] = (seq_t)1;          // initial state: no columns used

    for (idx_t i = 1; i <= n; ++i) {
        /* clear ndp */
        for (int m = 0; m < 16; ++m) ndp[m] = (seq_t)0;

        for (int m = 0; m < 16; ++m) {
            seq_t ways = dp[m];
            if (ways == (seq_t)0) continue;

            /* try assigning this row to any free column in the window */
            for (int k = 0; k < 4; ++k) {
                if (m & (1u << k)) continue;              // column already used
                int used = m | (1u << k);                  // mark it used
                int newmask = used >> 1;                   // window shifts left by 1
                ndp[newmask] += ways;
            }
        }
        /* copy ndp to dp for next iteration */
        for (int m = 0; m < 16; ++m) dp[m] = ndp[m];
    }

    /* Sum over all final states to get total number of matchings */
    seq_t result = (seq_t)0;
    for (int m = 0; m < 16; ++m) result += dp[m];
    return result;
}
