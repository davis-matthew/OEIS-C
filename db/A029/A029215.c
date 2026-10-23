/*
 * A029215  Expansion of 1/((1-x^2)*(1-x^6)*(1-x^7)*(1-x^9)).
 * https://oeis.org/A029215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 655, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A029215: Number of partitions of n into parts 2, 6, 7 and 9
   Computed by a straightforward coin‑change DP (order independent). */

#include <stdlib.h>

static const idx_t _parts[] = { 2, 6, 7, 9 };
static const size_t _part_count = 4;

seq_t a(idx_t n)
{
    /* Allocate space for dp[0..n] */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (dp == NULL)
        return (seq_t)0;             /* fallback if allocation fails */
    dp[0] = (seq_t)1;

    /* Classic coin‑change DP: outer loop over parts,
       inner loop over amount */
    for (size_t pi = 0; pi < _part_count; ++pi) {
        idx_t p = _parts[pi];
        for (idx_t i = p; i <= n; ++i)
            dp[i] += dp[i - p];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
