/*
 * A024940  Number of partitions of n into distinct triangular numbers.
 * https://oeis.org/A024940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 303, u16 n <= 1106, u32 n <= 4959, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A024940 – number of partitions of n into distinct triangular numbers.
   The implementation counts the subsets of the set {1,3,6,10,15,…} that sum to n
   using a 0‑1 knapsack dynamic‑programming approach.  For each triangular number t
   we update dp[s] += dp[s‑t] for all s from n down to t.  The result is returned
   in a seq_t value.  The algorithm runs in O(n·√n) time and O(n) memory. */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* allocate array dp[0..n] */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;            /* out of memory – unlikely for n ≤ 10000 */
    memset(dp, 0, (n + 1) * sizeof(seq_t));
    dp[0] = 1;                    /* empty sum */

    /* iterate over triangular numbers t = k*(k+1)/2 while t ≤ n */
    for (idx_t k = 1;; ++k)
    {
        /* compute triangular number as seq_t to avoid overflow */
        seq_t t = ((seq_t)k * (k + 1)) / 2;
        if (t > (seq_t)n) break;   /* no larger triangular numbers fit */
        idx_t tri = (idx_t)t;      /* fits in idx_t because t <= n */

        /* 0‑1 knapsack: add this triangular number once */
        for (idx_t s = n; s >= tri; --s)
            dp[s] += dp[s - tri];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
