/*
 * A029418  Expansion of 1/((1-x^6)*(1-x^7)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A029418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 6, 7, 9, and 11 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate temporary DP array */
    size_t N = (size_t)n;
    seq_t *dp = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp) return (seq_t)0;            /* fallback on allocation failure */

    dp[0] = (seq_t)1;                     /* base case: empty partition */

    /* Parts that may be used in the partition */
    const idx_t coins[4] = {6, 7, 9, 11};

    for (int c = 0; c < 4; ++c) {
        size_t coin = (size_t)coins[c];
        if (coin > N) continue;
        for (size_t i = coin; i <= N; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
