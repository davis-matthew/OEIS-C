/*
 * A109707  Number of partitions of n into parts each equal to 5 mod 7.
 * https://oeis.org/A109707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts ≡5 (mod 7)
 * Range:     u8 n <= 148, u16 n <= 352, u32 n <= 965, u64 n <= 3020,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Dynamic programming array for partition counts */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (dp == NULL) return (seq_t)0;          /* very unlikely, but safe */

    for (size_t i = 0; i < sz; ++i) dp[i] = 0;
    dp[0] = 1;

    for (idx_t part = 5; part <= n; part += 7) {
        for (idx_t i = part; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
