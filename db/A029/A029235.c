/*
 * A029235  Expansion of 1/((1-x^2)*(1-x^7)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 125, u16 n <= 883, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming: partitions of n using parts 2, 7, 11, 12.
   a(n) = number of integer solutions to n = 2x + 7y + 11z + 12w with x,y,z,w ≥ 0. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate DP array of size n+1 */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;   /* memory failure – unlikely for the limits */

    /* initialise */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* coin denominations in non‑decreasing order */
    static const unsigned parts[4] = {2, 7, 11, 12};

    /* dynamic programming */
    for (int k = 0; k < 4; ++k) {
        unsigned p = parts[k];
        for (idx_t i = p; i <= n; ++i)
            dp[i] += dp[i - p];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
