/*
 * A025462  Number of partitions of n into 9 positive cubes.
 * https://oeis.org/A025462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 9 positive cubes; obtained
 *            by a DP equivalent to the coefficient of y^9 x^n in ∏_{k≥1} 1/(1
 *            - y x^{k^3})
 * Range:     u8 n <= 9639, u16 n <= 9192, u32 n <= 9192, u64 n <= 9192,
 *            u128 n <= 9192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* number of parts we want */
    const int C = 10;               /* index 0..9 (inclusive) */
    size_t total = (size_t)(n + 1) * C;
    seq_t *dp = (seq_t *)malloc(total * sizeof(seq_t));
    if (!dp) return 0; /* fallback, unlikely for small n */

    for (size_t i = 0; i < total; ++i)
        dp[i] = (seq_t)0;

    dp[0] = (seq_t)1;              /* dp[0][0] = 1 */

    /* iterate over all cube values k^3 <= n */
    for (idx_t k = 1; k * k * k <= n; ++k) {
        idx_t v = k * k * k;        /* cube value */
        for (idx_t s = v; s <= n; ++s) {
            size_t base    = (size_t)s * C;
            size_t prevBase = (size_t)(s - v) * C;
            for (int c = 1; c <= 9; ++c) {
                /* dp[s][c] += dp[s - v][c - 1] */
                dp[base + c] += dp[prevBase + (c - 1)];
            }
        }
    }

    seq_t result = dp[(size_t)n * C + 9];
    free(dp);
    return result;
}
