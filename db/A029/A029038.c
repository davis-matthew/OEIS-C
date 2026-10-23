/*
 * A029038  Expansion of 1/((1-x)*(1-x^3)*(1-x^4)*(1-x^12)).
 * https://oeis.org/A029038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 3, 4, and 12
 * Range:     u8 n <= 50, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
static const idx_t coins[4] = {1, 3, 4, 12};

seq_t a(idx_t n)
{
    /* allocate DP array of length n+1, zero‑initialized */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t*)calloc(len, sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* out of memory – return zero */

    dp[0] = (seq_t)1;                  /* base case: one way to partition 0 */

    for (int ci = 0; ci < 4; ++ci) {
        idx_t v = coins[ci];
        for (idx_t i = v; i <= n; ++i) {
            dp[(size_t)i] += dp[(size_t)(i - v)];
        }
    }

    seq_t ans = dp[(size_t)n];
    free(dp);
    return ans;
}
