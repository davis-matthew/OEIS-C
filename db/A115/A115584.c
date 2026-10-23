/*
 * A115584  Number of partitions of n in which each part k occurs more than k
 *          times.
 * https://oeis.org/A115584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of partitions of n in which each part k occurs
 *            more than k times (computes using DP derived from the generating
 *            function ∏_{k≥1}(1‑x^k + x^{k(k+1)})/(1‑x^k) and the recurrence
 *            a(n)=a_prev(n)+a_prev(n‑k(k+1))+a_new(n‑k(k+1)-k)+… ). Uses
 *            O(n·√n) time and O(n) memory
 * Range:     u8 n <= 49, u16 n <= 140, u32 n <= 475, u64 n <= 1877,
 *            u128 n <= 8411, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array for sums up to n */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) return (seq_t)0; /* allocation failure – unlikely */
    dp[0] = (seq_t)1; /* empty partition */

    /* Temporary array to accumulate contributions that use part k at least k+1 times */
    seq_t *add = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!add) { free(dp); return (seq_t)0; }

    for (idx_t k = 1; ; ++k)
    {
        idx_t m0 = k * (k + 1);            /* minimal weight for part k (k+1 copies) */
        if (m0 > n) break;                 /* no more useful parts */

        /* reset 'add' array to zero up to n */
        for (idx_t i = 0; i <= n; ++i) add[i] = (seq_t)0;

        /* Compute contributions for using part k at least once (m0) and any extra copies */
        for (idx_t s = m0; s <= n; ++s)
        {
            seq_t base = dp[s - m0];
            seq_t prev = (s > k) ? add[s - k] : (seq_t)0;
            add[s] = base + prev;
        }

        /* Merge these contributions into dp for the next iteration */
        for (idx_t s = 0; s <= n; ++s) dp[s] += add[s];
    }

    seq_t result = dp[n];
    free(add);
    free(dp);
    return result;
}
