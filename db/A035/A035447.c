/*
 * A035447  Number of partitions of n into parts 8k or 8k+7.
 * https://oeis.org/A035447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 8k or 8k+7
 * Range:     u8 n <= 93, u16 n <= 226, u32 n <= 608, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* only defined for n >= 1 */
    /* Allocate DP array of size n+1 */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) return 0;                  /* memory allocation failure */
    dp[0] = (seq_t)1;                   /* empty partition */

    /* Process all parts that are multiples of 8 */
    for (idx_t s = 8; s <= n; s += 8) {
        for (idx_t j = s; j <= n; ++j) {
            dp[j] = dp[j] + dp[j - s];
        }
    }
    /* Process all parts that are 7 mod 8 */
    for (idx_t s = 7; s <= n; s += 8) {
        for (idx_t j = s; j <= n; ++j) {
            dp[j] = dp[j] + dp[j - s];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
