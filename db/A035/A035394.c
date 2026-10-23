/*
 * A035394  Number of partitions of n into parts 7k or 7k+5.
 * https://oeis.org/A035394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 7k or 7k+5
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but guard for safety */
    /* Allocate DP table of size n+1.  dp[i] will hold the number of partitions
       of i using allowed parts. */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;            /* fallback if allocation fails */

    dp[0] = (seq_t)1;              /* empty partition */
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* Add parts that are multiples of 7: 7, 14, 21, ... */
    for (idx_t k = 1; 7 * k <= n; ++k) {
        idx_t part = 7 * k;
        for (idx_t i = part; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - part];
        }
    }

    /* Add parts that are 5 modulo 7: 5, 12, 19, ... */
    for (idx_t k = 0; 7 * k + 5 <= n; ++k) {
        idx_t part = 7 * k + 5;
        for (idx_t i = part; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
