/*
 * A029011  Expansion of 1/((1-x)*(1-x^2)*(1-x^5)*(1-x^6)).
 * https://oeis.org/A029011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 2, 5, 6
 * Range:     u8 n <= 38, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;      /* memoised table of partition counts */
    static idx_t  max_n = 0;      /* largest index already computed */

    /* Initialise table for n = 0 if this is the first call */
    if (dp == NULL)
    {
        dp = (seq_t *)calloc(1, sizeof(seq_t));
        if (!dp) return (seq_t)0;          /* allocation failure */
        dp[0] = (seq_t)1;                  /* partition of 0 */
        max_n = 0;
    }

    /* If we already know this value (or a larger one) just return it */
    if (n <= max_n)
        return dp[n];

    /* Allocate a new table large enough for the requested n */
    seq_t *new_dp = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!new_dp) return (seq_t)0;          /* allocation failure */

    new_dp[0] = (seq_t)1;                  /* base case for all n */

    /* Compute partitions using the standard coin‑change DP algorithm */
    const idx_t parts[4] = {1, 2, 5, 6};
    for (int pi = 0; pi < 4; ++pi)
    {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= n; ++i)
            new_dp[i] += new_dp[i - p];
    }

    /* Replace the old table */
    free(dp);
    dp = new_dp;
    max_n = n;

    return dp[n];
}
