/*
 * A036587  Number of binary rooted trees with n nodes and height at most 4.
 * https://oeis.org/A036587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of unordered binary rooted trees with n nodes and
 *            * height at most 4, computed by dynamic programming. * * The
 *            recurrence for the number of trees of height ≤ h is * * U[h][0]
 *            = 1 * U[h][1] = 1 (leaf) * U[h][n] = U[h-1][n-1] (one child) * +
 *            Σ_{1 ≤ i ≤ floor((n-1)/2)}[ * if i < n-1-i...
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* sizes needed: height 0..4, nodes 0..15 */
    const idx_t MAX_N = 15;
    static seq_t dp[5][16];
    static bool initialized = false;

    if (!initialized) {
        /* base for height 0 */
        dp[0][0] = 1;
        for (idx_t i = 1; i <= MAX_N; ++i) dp[0][i] = 0;

        for (idx_t h = 1; h <= 4; ++h) {
            dp[h][0] = 1;          /* empty tree */
            for (idx_t n = 1; n <= MAX_N; ++n) {
                seq_t val = 0;
                /* leaf */
                if (n == 1) {
                    val = 1;
                }
                if (n >= 2) {
                    /* one child */
                    val += dp[h-1][n-1];

                    /* two children, unordered pairs */
                    idx_t max_i = (n - 1) / 2;
                    for (idx_t i = 1; i <= max_i; ++i) {
                        idx_t j = n - 1 - i;
                        if (i < j) {
                            val += dp[h-1][i] * dp[h-1][j];
                        } else { /* i == j */
                            seq_t x = dp[h-1][i];
                            val += (x * (x + 1)) / 2;
                        }
                    }
                }
                dp[h][n] = val;
            }
        }
        initialized = true;
    }

    if (n > MAX_N) return 0;
    return dp[4][n];
}
