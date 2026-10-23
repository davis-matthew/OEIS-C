/*
 * A008631  Molien series for alternating group Alt_8 (or A_8).
 * https://oeis.org/A008631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 57, u32 n <= 325, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A008631: number of partitions of n into parts 1..8 with at most one 28 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    idx_t size = n + 1;
    seq_t *dp = (seq_t*)malloc((size_t)size * sizeof(seq_t));
    if (!dp) return (seq_t)0; /* allocation failure */

    dp[0] = (seq_t)1;
    for (idx_t i = 1; i < size; ++i) dp[i] = (seq_t)0;

    for (idx_t part = 1; part <= 8; ++part) {
        for (idx_t j = part; j < size; ++j) {
            dp[j] = dp[j] + dp[j - part];
        }
    }

    seq_t res = dp[n];
    if (n >= 28) res = res + dp[n - 28];

    free(dp);
    return res;
}
