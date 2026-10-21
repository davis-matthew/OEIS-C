/*
 * A238479  Number of partitions of n whose median is not a part.
 * https://oeis.org/A238479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..floor(n/3)} p_{≤2k}(n-3k)
 * Range:     u8 n <= 22, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p_{≤m}(t) is the number of partitions of t using parts ≤ m.  */

#include <stdlib.h>

/* Cached data for the largest n already processed. */
static seq_t *dp = NULL;          /* dp[s] = partitions of s using parts ≤ current max. */
static seq_t *ans = NULL;         /* answer array for all n ≤ curMaxN. */
static idx_t curMaxN = 0;         /* largest n processed so far. */

/* Recompute dp and ans for all n up to Nmax. */
static void recompute(idx_t Nmax)
{
    /* Allocate new arrays. */
    free(dp);
    free(ans);
    dp  = (seq_t *)malloc((Nmax + 1) * sizeof(seq_t));
    ans = (seq_t *)malloc((Nmax + 1) * sizeof(seq_t));

    /* Initialise to zero. */
    for (idx_t i = 0; i <= Nmax; ++i) {
        dp[i]  = (seq_t)0;
        ans[i] = (seq_t)0;
    }
    dp[0] = (seq_t)1;    /* empty partition of 0 */

    /* Maximum part size that can appear in the formula: 2*floor(Nmax/3).  */
    idx_t max_part = (Nmax / 3) * 2;

    /* Main loop over part sizes k = 1 .. max_part.  */
    for (idx_t k = 1; k <= max_part; ++k) {
        /* Update partitions: add a part of size k. */
        for (idx_t s = k; s <= Nmax; ++s) {
            dp[s] += dp[s - k];
        }

        /* For even k, add contributions to ans[n]. */
        if ((k & 1u) == 0u) {
            idx_t m = k >> 1;            /* m = k/2. */
            idx_t start = 3 * m;          /* n = 3m + t, t = n - 3m. */
            if (start > Nmax) continue;  /* nothing to add for this k. */
            for (idx_t n = start; n <= Nmax; ++n) {
                ans[n] += dp[n - start];
            }
        }
    }

    curMaxN = Nmax;
}

/* Public function requested by the build system. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* not defined for 0, return 0. */

    if (n > curMaxN) {
        recompute(n);
    }
    /* n is now within the precomputed range. */
    return ans[n];
}
