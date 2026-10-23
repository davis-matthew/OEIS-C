/*
 * A035435  Number of partitions of n into parts 7k+3 or 7k+4.
 * https://oeis.org/A035435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into parts 7k+3 or 7k+4
 * Range:     u8 n <= 61, u16 n <= 153, u32 n <= 443, u64 n <= 1434,
 *            u128 n <= 5029, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate workspace for dynamic programming */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;           /* allocation failed – unlikely for tests */

    /* initialise: dp[0] = 1 (empty partition), others 0 */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* add parts congruent to 3 (mod 7) */
    for (idx_t p = 3; p <= n; p += 7) {
        for (idx_t j = p; j <= n; ++j)
            dp[j] += dp[j - p];
    }

    /* add parts congruent to 4 (mod 7) */
    for (idx_t p = 4; p <= n; p += 7) {
        for (idx_t j = p; j <= n; ++j)
            dp[j] += dp[j - p];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
