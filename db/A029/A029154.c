/*
 * A029154  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^6)*(1-x^11)).
 * https://oeis.org/A029154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 3, 6, and 11
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for dynamic programming */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (dp == NULL)
        return 0; /* fall back on memory allocation failure */

    dp[0] = 1; /* empty partition */

    /* Parts allowed in increasing order */
    static const idx_t parts[4] = {2, 3, 6, 11};

    /* Classic coin‑change DP: parts outer, j inner, increasing j */
    for (int pi = 0; pi < 4; ++pi) {
        idx_t part = parts[pi];
        for (idx_t j = part; j <= n; ++j)
            dp[j] += dp[j - part];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
