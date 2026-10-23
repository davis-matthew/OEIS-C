/*
 * A029355  Expansion of 1/((1-x^4)*(1-x^6)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A029355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A029355: Number of partitions of n into parts 4, 6, 9 and 11.
   Computed with a simple dynamic‑programming routine. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static table of values up to current maximum index. */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* Expand the table if needed. */
    if (dp_size <= n) {
        idx_t new_size = n + 1;
        if (dp_size == 0) {
            dp = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        }
        dp_size = new_size;
    }

    /* Re‑initialize the table before recomputing. */
    for (idx_t i = 0; i < dp_size; ++i) dp[i] = 0;
    dp[0] = 1;

    /* The allowed parts. */
    const idx_t parts[4] = {4, 6, 9, 11};
    const int pcnt = 4;

    /* Dynamic programming: for each part, add contributions. */
    for (int pi = 0; pi < pcnt; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i < dp_size; ++i) {
            dp[i] += dp[i - p];
        }
    }

    return dp[n];
}
