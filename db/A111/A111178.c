/*
 * A111178  Number of partitions of n into positive numbers one less than a
 *          square.
 * https://oeis.org/A111178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Number of partitions of n into parts k^2-1, k >= 2
 * Range:     u8 n <= 140, u16 n <= 409, u32 n <= 1505, u64 n <= 6835,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, calloc, free */

seq_t a(idx_t n)
{
    /* Allocate DP table of size n+1 */
#ifdef __cplusplus
    seq_t *dp = new seq_t[n+1]();   /* value-initialized to zero */
#else
    seq_t *dp = (seq_t*)calloc((size_t)(n+1), sizeof(seq_t)); /* zeroed */
#endif
    if (!dp) return 0;               /* out of memory */

    dp[0] = 1;                       /* one way to sum to zero */

    /* Iterate over all allowed part sizes p = k^2 - 1, k >= 2 */
    for (idx_t k = 2; ; ++k) {
        idx_t p = k * k - 1;
        if (p > n) break;
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];

    /* Free the table */
#ifdef __cplusplus
    delete [] dp;
#else
    free(dp);
#endif
    return result;
}
