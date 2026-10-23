/*
 * A161060  Number of partitions of n into powers of two where every part
 *          appears at least 11 times.
 * https://oeis.org/A161060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into powers of two where each part
 *            appears at least 11 times
 * Range:     u8 n <= 125, u16 n <= 409, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate arrays for DP */
    seq_t *dp   = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *base = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp || !base) { free(dp); free(base); return 0; }

    dp[0] = 1; /* one way to make 0 */

    for (idx_t r = 1; r <= n; r <<= 1)   /* iterate powers of 2 */
    {
        /* zero base array */
        for (idx_t i = 0; i <= n; ++i) base[i] = 0;

        /* first 11 copies of the coin */
        idx_t shift = 11 * r;
        for (idx_t s = shift; s <= n; ++s)
            base[s] = dp[s - shift];

        /* add unlimited additional copies of the coin */
        for (idx_t s = r; s <= n; ++s)
            base[s] += base[s - r];

        /* add the contributions from this coin to the running total */
        for (idx_t s = 0; s <= n; ++s)
            dp[s] += base[s];
    }

    seq_t result = dp[n];
    free(dp);
    free(base);
    return result;
}
