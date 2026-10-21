/*
 * A095699  Number of partitions of n into generalized pentagonal numbers.
 * https://oeis.org/A095699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into generalized pentagonal
 *            numbers
 * Range:     u8 n <= 31, u16 n <= 107, u32 n <= 443, u64 n <= 2185,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#if defined(__cplusplus)
#include <new>
#endif

seq_t a(idx_t n)
{
    /* Static DP array */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* Allocate or grow array if required */
    if (n + 1 > dp_size) {
        idx_t new_size = n + 1;
        if (dp) {
#if defined(__cplusplus)
            delete [] dp;
#else
            free(dp);
#endif
        }
#if defined(__cplusplus)
        dp = new seq_t[new_size];
#else
        dp = (seq_t*)malloc(new_size * sizeof(seq_t));
#endif
        if (!dp) return 0;              /* allocation failure */
        dp_size = new_size;
    }

    /* Initialize dp[0] = 1 and the rest with 0 */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* DP over generalized pentagonal numbers */
    for (idx_t k = 1; ; ++k) {
        idx_t g1 = k * ((idx_t)3 * k - 1) / 2;   /* k(3k-1)/2 */
        idx_t g2 = k * ((idx_t)3 * k + 1) / 2;   /* k(3k+1)/2 */

        if (g1 > n && g2 > n)
            break;

        if (g1 <= n) {
            for (idx_t j = g1; j <= n; ++j)
                dp[j] += dp[j - g1];
        }
        if (g2 <= n) {
            for (idx_t j = g2; j <= n; ++j)
                dp[j] += dp[j - g2];
        }
    }

    return dp[n];
}
