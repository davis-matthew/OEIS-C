/*
 * A029295  Expansion of 1/((1-x^3)*(1-x^6)*(1-x^7)*(1-x^11)).
 * https://oeis.org/A029295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 3, 6, 7, and 11 */
/* Computed by simple dynamic programming (coin–change combinations) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array for values 0 .. n */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;     /* Out of memory – return zero */

    /* initialise with zero, except dp[0] = 1 (empty partition) */
    for (size_t i = 0; i < len; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* The available part sizes */
    const idx_t parts[4] = {3, 6, 7, 11};

    /* For each part, add combinations to all larger totals */
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
