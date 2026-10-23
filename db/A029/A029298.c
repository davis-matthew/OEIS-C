/*
 * A029298  Expansion of 1/((1-x^3)*(1-x^6)*(1-x^8)*(1-x^10)).
 * https://oeis.org/A029298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 6, 8, and 10
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate and zero‑initialize an array of length n+1 */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp) return 0;                 /* allocation failure fallback */

    dp[0] = 1;                          /* one way to make sum 0 */

    /* The allowed parts */
    const idx_t parts[4] = {3, 6, 8, 10};

    for (int pi = 0; pi < 4; ++pi) {
        idx_t part = parts[pi];
        for (idx_t i = part; i <= n; ++i) {
            dp[(size_t)i] += dp[(size_t)(i - part)];
        }
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
