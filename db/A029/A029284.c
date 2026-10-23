/*
 * A029284  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^8)*(1-x^11)).
 * https://oeis.org/A029284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 5, 8 and 11
 * Range:     u8 n <= 113, u16 n <= 790, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* array length is n+1; use size_t to avoid signed overflow */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp)
        return 0;          /* allocation failed – should never happen */

    /* initialise */
    for (size_t i = 0; i < len; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* the allowed part sizes */
    const idx_t parts[4] = {3, 5, 8, 11};
    const size_t partcnt = 4;

    /* dynamic programming: for each part, add its contribution */
    for (size_t pi = 0; pi < partcnt; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
