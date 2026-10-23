/*
 * A132015  Number of partitions of n into distinct parts such that u^2 < v
 *          for all pairs (u,v) of parts with u<v.
 * https://oeis.org/A132015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts such that
 *            for all pairs of parts u < v we have u^2 < v
 * Range:     u8 n <= 1264, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation enumerates all admissible increasing part
   sequences.  For each partition its sum is counted in a dynamic array
   dp[sum] giving a(n) = dp[n].  No memoised recurrence is used; the
   number of admissible parts grows extremely fast, so the enumeration
   is extremely fast for n up to at least 10 000.  The algorithm runs in
   well under 2 s for all provided inputs. */

#include <stdlib.h>
#include <stdint.h>

/* static helper performing a depth‑first search of admissible parts */
static void
dfs(seq_t *dp, size_t max_n, size_t sum, size_t last)
{
    /* Next part must be strictly larger than last^2 */
    size_t next_min = (last * last) + 1;
    /* We may only add a part if it does not exceed the remaining capacity */
    size_t remaining = max_n - sum;
    if (next_min > remaining)
        return;
    for (size_t q = next_min; q <= remaining; ++q) {
        size_t new_sum = sum + q;
        dp[new_sum] += (seq_t)1;
        dfs(dp, max_n, new_sum, q);
    }
}

/* static storage of the sequence values up to the greatest n computed */
static seq_t *dp = NULL;            /* dp[0] unused */
static size_t dp_cap = 0;           /* size of allocated dp array */

/* The required API function */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;            /* offset is 1, values for n=0 are undefined */

    size_t target = (size_t)n;      /* target sum to compute up to */

    /* Recompute the table if we need a larger size */
    if (dp_cap < target + 1) {
        /* Reallocate array to the new size and zero it */
        seq_t *tmp = (seq_t *)realloc(dp, (target + 1) * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;        /* allocation failed */
        dp = tmp;
        for (size_t i = 0; i <= target; ++i)
            dp[i] = (seq_t)0;

        /* Enumerate all admissible partitions of every sum <= target */
        for (size_t part = 1; part <= target; ++part) {
            dp[part] += (seq_t)1;   /* partition consisting of the single part */
            dfs(dp, target, part, part);
        }
        dp_cap = target + 1;
    }

    return dp[target];
}
