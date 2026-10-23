/*
 * A025840  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^12)).
 * https://oeis.org/A025840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 5, 12
 * Range:     u8 n <= 290, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation via dynamic programming on the coin set {3,5,12}. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t *dp = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    dp[0] = 1;

    const idx_t parts[3] = {3, 5, 12};
    for (int p = 0; p < 3; ++p) {
        idx_t coin = parts[p];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
