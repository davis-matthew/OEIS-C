/*
 * A035990  Number of partitions of n into parts not of the form 23k, 23k+2 or
 *          23k-2. Also number of partitions with 1 part of size 1 and
 *          differences between parts at distance 10 are greater than 1.
 * https://oeis.org/A035990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not congruent to 0,
 *            2 or 21 modulo 23. Computed by dynamic programming
 * Range:     u8 n <= 20, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*  dp[i] holds the number of allowed partitions of i using parts
    ≤ the current maximum part.  The table is rebuilt from scratch
    whenever a larger n is requested. */
static seq_t *dp = NULL;   /* array of partition counts */
static idx_t computed = 0; /* largest n for which dp[0..n] is valid */

/* Rebuild the partition table for all sums up to n.  The algorithm
   iterates over each allowed part size and adds its contribution to
   the sums for which it is applicable. */
static void build_dp(idx_t n)
{
    /* Resize the table */
    size_t new_size = (size_t)n + 1;            /* indices 0..n */
    seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
    if (!tmp) return;                          /* allocation failed */
    dp = tmp;

    /* Base case: one way to partition 0 */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* Build the table */
    for (idx_t part = 1; part <= n; ++part)
    {
        idx_t r = part % 23;
        if (r == 0 || r == 2 || r == 21) continue;   /* forbidden part */
        for (idx_t sum = part; sum <= n; ++sum)
            dp[sum] = dp[sum] + dp[sum - part];
    }

    computed = n;
}

/* Public entry point – returns a(n) for the sequence.  The OEIS
   offset is 1, so a(1) is the first term in the table.  Calls with
   n == 0 return 0 as a degenerate case. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset protection */
    if (n > computed) build_dp(n);        /* recompute if needed */
    return dp[n];
}
