/*
 * A029274  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^6)*(1-x^9)).
 * https://oeis.org/A029274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 5, 6, and 9
 * Range:     u8 n <= 92, u16 n <= 668, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Allocate DP array of length n+1.  index 0..n. */
    if (n == (idx_t)0) return (seq_t)1;   /* a(0) = 1 */

    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;  /* allocation failure → 0 (should not happen) */

    for (idx_t i = (idx_t)0; i <= n; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    static const idx_t parts[4] = { (idx_t)3, (idx_t)5, (idx_t)6, (idx_t)9 };

    for (int p = 0; p < 4; ++p) {
        idx_t part = parts[p];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
