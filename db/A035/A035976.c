/*
 * A035976  Number of partitions of n into parts not of the form 19k, 19k+7 or
 *          19k-7. Also number of partitions with at most 6 parts of size 1
 *          and differences between parts at distance 8 are greater than 1.
 * https://oeis.org/A035976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP computing the number of partitions of n into parts not of the form
   19k, 19k+7, or 19k-7.  The sequence has OEIS offset 1 (a(1) is the
   first listed term).  The routine memoises the computed values so that
   repeated calls to a non‑monotonic sequence of indices are fast.  */

#include <stdlib.h>
#include <stdbool.h>

/* static data for memoisation */
static seq_t *dp = NULL;          /* dp[i] = number of partitions of i */
static idx_t  dpMax = 0;          /* largest n that has been computed */

seq_t a(idx_t n)
{
    if (n < 1)                 /* sequence is defined only for n ≥ 1 */
        return 0;

    /* If we need more values than we have computed, recompute the table
       up to the new maximum.  The cost is O(n^2), which is acceptable
       for n ≤ 10000 (≈ 100 million inner iterations). */
    if (n > dpMax) {
        size_t sz = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, sz * sizeof(seq_t));
        if (!tmp)         /* allocation failed – fallback to zero */
            return 0;
        dp = tmp;

        /* reset the table */
        for (size_t i = 0; i <= n; ++i)
            dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;   /* empty partition */

        /* DP: iterate over all admissible part sizes p in ascending order.
           For each part p we add its contribution to all totals s ≥ p. */
        for (idx_t p = 1; p <= n; ++p) {
            int r = (int)(p % 19);
            if (r == 0 || r == 7 || r == 12)   /* forbidden parts   */
                continue;
            for (idx_t s = p; s <= n; ++s)
                dp[s] += dp[s - p];
        }
        dpMax = n;
    }
    return dp[n];
}
