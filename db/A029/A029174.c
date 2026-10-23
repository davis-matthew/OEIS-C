/*
 * A029174  Expansion of 1/((1-x^2)*(1-x^4)*(1-x^5)*(1-x^9)).
 * https://oeis.org/A029174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of partitions of n into parts 2, 4, 5, and 9. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The parts allowed in the partitions. */
    static const idx_t parts[] = { 2, 4, 5, 9 };
    const size_t nparts = sizeof parts / sizeof parts[0];

    /* Work with size_t for array indexing. */
    size_t m = (size_t)n;

    /* Allocate the DP table and zero‑initialize it. */
    seq_t *dp = (seq_t *)calloc(m + 1, sizeof *dp);
    if (!dp) return 0;            /* allocation failed – return 0 */

    dp[0] = 1;   /* empty partition */

    for (size_t pi = 0; pi < nparts; ++pi) {
        size_t p = (size_t)parts[pi];
        for (size_t i = p; i <= m; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[m];
    free(dp);
    return result;
}
