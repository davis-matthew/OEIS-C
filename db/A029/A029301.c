/*
 * A029301  Expansion of 1/((1-x^3)*(1-x^6)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A029301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 6, 9 and 10 *
 *            Computation: classic coin‑change DP, sums the number of ways to
 *            * reach each value using the four part sizes
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Alloc a small temporary table of length n+1.  The table fits
     * comfortably into memory for n up to at least 10000. */
    size_t len = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;   /* allocation failure – should never happen here */

    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Iterate over the allowed part sizes.  For each part we update
     * the table so that dp[v] counts the ways to form value v using
     * parts from the set {3,6,9,10}. */
    const idx_t parts[4] = {3, 6, 9, 10};
    for (int p = 0; p < 4; ++p) {
        idx_t part = parts[p];
        for (idx_t v = part; v <= n; ++v)
            dp[v] += dp[v - part];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
