/*
 * A035991  Number of partitions in parts not of the form 23k, 23k+3 or 23k-3.
 *          Also number of partitions with at most 2 parts of size 1 and
 *          differences between parts at distance 10 are greater than 1.
 * https://oeis.org/A035991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035991
   Number of partitions of n into parts not congruent to 0, 3, or 20 modulo 23.
   Computed by ordinary dynamic programming:  dp[0]=1; for each allowed part p,
   for j=p..n: dp[j] += dp[j-p]. */

/* The sequence is indexed starting at 1; for n<1 the function returns 0. */
#include <stdlib.h>

/* Static storage for partition counts up to the largest n seen so far. */
static seq_t *dp = NULL;      /* dp[i] = a(i) for 0 <= i <= max_n */
static idx_t max_n = 0;       /* maximum index for which dp[] is valid */

/* Recompute all values up to n (inclusive).  Allocates or resizes the dp array. */
static void compute_upto(idx_t n)
{
    if (dp == NULL)
    {
        dp = (seq_t*)realloc(NULL, (size_t)(n + 1) * sizeof(seq_t));
    }
    else
    {
        seq_t *new_dp = (seq_t*)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_dp)   /* allocation failure – give up */
        {
            free(dp);
            dp = NULL;
            max_n = 0;
            return;
        }
        dp = new_dp;
    }

    /* Initialise the array.  dp[0] will be set to 1 below. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Dynamic programming over all allowed part sizes. */
    for (idx_t part = 1; part <= n; ++part)
    {
        idx_t r = part % 23;
        if (r == 0 || r == 3 || r == 20)   /* forbidden residues */
            continue;

        for (idx_t j = part; j <= n; ++j)
            dp[j] += dp[j - part];
    }

    max_n = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* out‑of‑range request */

    if (n > max_n)
        compute_upto(n);

    return dp[n];
}
