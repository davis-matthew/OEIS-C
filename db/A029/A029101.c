/*
 * A029101  Expansion of 1/((1-x)*(1-x^5)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A029101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 566, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of partitions of n into parts 1, 5, 9 and 11.
   Uses the standard coin‑change dynamic programming: each coin is
   processed once, adding its contribution to all larger amounts.
   This guarantees the correct count without double counting. */
#include <stdlib.h>

static const idx_t parts[4] = {1, 5, 9, 11};

seq_t a(idx_t n)
{
    /* The answer for n = 0 is the empty partition. */
    if (n == 0)
        return (seq_t)1;

    /* Allocate a zeroed DP table up to n. */
    size_t sz = (size_t)(n + 1);
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp)
        return (seq_t)0;                /* unlikely, but safe */

    dp[0] = (seq_t)1;                   /* one way to make 0 */

    /* Process each coin in turn. */
    for (int c = 0; c < 4; ++c) {
        idx_t coin = parts[c];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
