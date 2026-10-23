/*
 * A015742  Number of 7's in all the partitions of n into distinct parts.
 * https://oeis.org/A015742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 7's in all partitions of n into distinct parts.
 *            For n ≥ 7, a(n) equals the number of partitions of n−7 into
 *            distinct parts that do not contain the part 7. The following
 *            code pre‑computes the table of partitions without part 7 and
 *            returns the value in O(1)
 * Range:     u8 n <= 38, u16 n <= 89, u32 n <= 253, u64 n <= 817,
 *            u128 n <= 2866, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;          /* DP table: dp[i] = partitions of i into
                                    distinct parts not using 7      */
static idx_t  dp_cap = 0;         /* current DP size (dp_cap = max i + 1) */

/* Rebuild the whole DP table up to `max`.  Complexity O(max²). */
static void recompute_dp(idx_t max)
{
    seq_t *new_dp = (seq_t *)malloc((max + 1) * sizeof(seq_t));
    /* initialise entries to 0 */
    for (idx_t i = 0; i <= max; ++i) new_dp[i] = 0;
    new_dp[0] = 1;   /* empty partition */

    /* standard distinct‑parts DP: for each part j, add it once
       using a backwards loop to avoid repetitions. */
    for (idx_t j = 1; j <= max; ++j)
    {
        if (j == 7) continue;   /* skip part 7 */
        for (idx_t i = max; i >= j; --i)
            new_dp[i] += new_dp[i - j];
    }

    free(dp);
    dp      = new_dp;
    dp_cap  = max + 1;
}

seq_t a(idx_t n)
{
    if (n < 7) return (seq_t)0;

    idx_t m = n - 7;                 /* need number of partitions of n-7 */
    if (m >= dp_cap)                 /* grow table if necessary */
        recompute_dp(m);

    return dp[m];
}
