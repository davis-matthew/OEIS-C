/*
 * A025850  Expansion of 1/((1-x^3)*(1-x^8)*(1-x^9)).
 * https://oeis.org/A025850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 8, and 9.
 *            Computed by dynamic programming over the three coin sizes
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle the trivial case */
    if (n == 0u) return (seq_t)1;

    idx_t N = n;
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* memory failure – unlikely */

    for (idx_t i = 0; i <= N; ++i) dp[i] = 0;
    dp[0] = 1;                         /* empty partition */

    const idx_t coins[3] = {3, 8, 9};
    for (int c = 0; c < 3; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= N; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
