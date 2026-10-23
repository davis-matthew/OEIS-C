/*
 * A100853  Number of partitions of n in which every part occurs 1, 4, or 5
 *          times. Also number of partitions of n in which every part is
 *          congruent to {1, 3, 4, 5, 7} mod 8.
 * https://oeis.org/A100853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   dynamic programming for partitions in which every part occurs
 *            exactly 1, 4, or 5 times (equivalently, parts ≡ 1,3,4,5,7 (mod
 *            8)). a(0) = 1; a(n) is the number of such partitions of n
 * Range:     u8 n <= 23, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *dp = NULL;
    static idx_t dp_cap = 0;        /* allocated size of dp array   */
    static idx_t max_n_computed = 0;/* largest n for which dp[0..n] is up to date */

    /* Allocate or enlarge the dp array if necessary */
    if (dp_cap < n + 1) {
        seq_t *new_dp = (seq_t *)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_dp) return (seq_t)0; /* allocation failed */
        dp = new_dp;
        dp_cap = n + 1;
    }

    /* Recompute the table whenever a larger or equal n is requested.
       This guarantees that dp[0] is always initialized correctly. */
    if (n >= max_n_computed) {
        /* Zero the relevant part of the array */
        for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;                /* base case */

        /* Core dynamic‑programming loops */
        for (idx_t k = 1; k <= n; ++k) {
            for (idx_t t = n; t >= k; --t) {
                dp[t] += dp[t - k];                /* add one copy of part k */
                if (t >= 4 * k) dp[t] += dp[t - 4 * k]; /* add four copies   */
                if (t >= 5 * k) dp[t] += dp[t - 5 * k]; /* add five copies   */
            }
        }
        max_n_computed = n;
    }

    return dp[n];
}
