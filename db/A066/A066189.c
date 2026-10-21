/*
 * A066189  Sum of all partitions of n into distinct parts.
 * https://oeis.org/A066189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * q(n), where q(n) is the number of partitions of n
 *            into distinct parts
 * Range:     u8 n <= 13, u16 n <= 42, u32 n <= 155, u64 n <= 600,
 *            u128 n <= 2377, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;

    /* Allocate and zero‑initialize the DP array for counts of distinct partitions */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return (seq_t)0;           /* Allocation failed: return 0 as fallback */

    dp[0] = (seq_t)1;                   /* Empty partition */

    /* Classic 1‑dimension DP: for each part size i, add it to all larger sums */
    for (idx_t part = 1u; part <= n; ++part) {
        for (idx_t s = n; s >= part; --s) {
            dp[s] += dp[s - part];
        }
    }

    seq_t result = ((seq_t)n) * dp[n];  /* a(n) = n * q(n) */

    free(dp);
    return result;
}
