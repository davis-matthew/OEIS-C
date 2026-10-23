/*
 * A167700  Number of partitions of n into distinct odd squares.
 * https://oeis.org/A167700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of partitions of n into distinct odd squares
 *            (weights w_k = (2k+1)^2, each used at most once)
 * Range:     u8 n <= 299, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* dp[s] = number of ways to reach sum s with distinct odd squares */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* ensure dp array has size n+1 */
    if (dp_size < n + 1) {
        if (dp) free(dp);
        dp = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
        dp_size = n + 1;
    } else {
        /* reset values for indices 0..n */
        for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    }

    dp[0] = 1;   /* one way to make sum 0: take nothing */

    /* iterate over odd squares */
    for (idx_t k = 0; ; ++k) {
        seq_t odd = (seq_t)(2 * k + 1);
        seq_t w = odd * odd;              /* w = (2k+1)^2 */
        if (w > (seq_t)n) break;          /* larger weights cannot be used */

        idx_t ws = (idx_t)w;              /* safe: w <= n <= idx_t */
        /* 0/1 knapsack style DP – iterate sums downward */
        for (idx_t s = n; s >= ws; --s) {
            dp[s] = dp[s] + dp[s - ws];
        }
    }

    return dp[(idx_t)n];
}
