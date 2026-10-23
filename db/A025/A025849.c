/*
 * A025849  Expansion of 1/((1-x^3)*(1-x^7)*(1-x^12)).
 * https://oeis.org/A025849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* A025849: number of partitions of n into parts 3, 7 and 12.
   The function computes the value via dynamic programming:
   for each part p, it adds dp[i] += dp[i-p] for i from p to n. */

seq_t a(idx_t n)
{
    /* allocate a temporary table of size n+1 */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) abort();          /* out of memory */

    dp[0] = 1;                  /* empty partition */

    static const int parts[3] = {3, 7, 12};

    for (int pi = 0; pi < 3; ++pi) {
        idx_t p = (idx_t)parts[pi];
        if (p > n) continue;
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
