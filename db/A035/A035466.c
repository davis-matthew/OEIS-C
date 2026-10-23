/*
 * A035466  Number of partitions of n into parts 8k+5 or 8k+6.
 * https://oeis.org/A035466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into parts 8k+5 or 8k+6
 * Range:     u8 n <= 86, u16 n <= 202, u32 n <= 557, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: dp[i] = number of partitions of i */
    static seq_t *dp = NULL;
    static idx_t max_n = 0;

    if (n > max_n) {
        /* free old cache if any */
        if (dp) free(dp);

        /* allocate new cache (size n+1, index 0..n) */
        dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!dp) return (seq_t)0;              /* out of memory, return 0 */

        /* initialize */
        dp[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i)
            dp[i] = (seq_t)0;

        /* add parts congruent to 5 mod 8 */
        for (idx_t k = 0; 8*k + 5 <= n; ++k) {
            idx_t part = 8*k + 5;
            for (idx_t i = part; i <= n; ++i)
                dp[i] += dp[i - part];
        }

        /* add parts congruent to 6 mod 8 */
        for (idx_t k = 0; 8*k + 6 <= n; ++k) {
            idx_t part = 8*k + 6;
            for (idx_t i = part; i <= n; ++i)
                dp[i] += dp[i - part];
        }

        max_n = n;
    }

    return dp[n];
}
