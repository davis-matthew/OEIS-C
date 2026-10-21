/*
 * A218571  Number of partitions p of n such that max(p)-min(p) = 8.
 * https://oeis.org/A218571
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 61, u32 n <= 281, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A218571: number of partitions of n with max(p)-min(p) = 8.
 *
 *  For each possible minimal part k (1 <= k <= (n-8)/2) we count the
 *  partitions of the remaining amount
 *        n - 2*k - 8
 *  using parts in the inclusive range [k, k+8].
 *  This is a standard unbounded coin‑change DP.  The sum over all k is the
 *  desired value.  The implementation uses a single DP array sized n+1
 *  and is valid for any seq_t (unsigned integer or arbitrary‑precision
 *  class).  The complexity is about O(n^2) with a small constant.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* For indices below the offset the sequence is undefined; return 0. */
    if (n < 10)
        return 0;

    /* Upper bound for DP array dimension. */
    size_t Nmax = (size_t)n;

    /* Allocate one DP array of size n+1 and zero‑initialize it. */
    seq_t *dp = (seq_t *)calloc(Nmax + 1, sizeof(seq_t));
    if (!dp)
        return 0;   /* allocation failure – undefined behaviour */

    seq_t result = 0;
    idx_t max_k = (n - 8) / 2;          /* largest possible minimal part */

    for (idx_t k = 1; k <= max_k; ++k)
    {
        idx_t target = n - 2 * k - 8;
        if (target < 0)   /* should not happen for k <= max_k */
            continue;

        size_t tgt = (size_t)target;

        /* reset dp[0..tgt]: dp[0]=1, others 0 */
        dp[0] = 1;
        for (size_t s = 1; s <= tgt; ++s)
            dp[s] = 0;

        /* unbounded coin change over coins k .. k+8, but at most target */
        for (idx_t coin = k; coin <= k + 8 && coin <= target; ++coin)
        {
            size_t c = (size_t)coin;
            for (size_t sum = c; sum <= tgt; ++sum)
                dp[sum] += dp[sum - c];
        }

        result += dp[tgt];
    }

    free(dp);
    return result;
}
