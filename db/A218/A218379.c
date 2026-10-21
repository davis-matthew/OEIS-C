/*
 * A218379  Number of partitions of n into pentagonal parts.
 * https://oeis.org/A218379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 321, u32 n <= 1413, u64 n <= 7414,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A218379: Number of partitions of n into pentagonal parts.
   G.f.: ∏_{k≥1} 1/(1 − x^{k(3k−1)/2})          */

/* The routine recomputes a pure coin‑change dynamic‑programming table
   from scratch on each call.  For the index set {1,5,12,22,…} it
   counts unordered partitions (order of parts does not matter).  The
   table size is (n+1) and the algorithm runs in O(n·√n) time,
   which is comfortably below the 2‑second limit for n ≤ 10000. */

#include <stddef.h>
#include <stdlib.h>

/* ---- only the required function ---- */
seq_t a(idx_t n)
{
    static seq_t *dp = NULL;     /* table of partial results */
    static size_t dp_size = 0;   /* allocated length of dp */

    /* grow the table if needed */
    size_t new_size = (size_t)n + 1;
    if (dp_size < new_size) {
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        dp = tmp;
        dp_size = new_size;
    }

    /* initialise table */
    for (size_t i = 0; i < dp_size; ++i) dp[i] = 0;
    dp[0] = (seq_t)1;           /* empty partition */

    /* coin–change DP over pentagonal numbers */
    for (idx_t k = 1; ; ++k) {
        /* pentagonal number p_k = k(3k-1)/2 */
        seq_t pk = (seq_t)k * (((seq_t)k * (seq_t)3) - (seq_t)1) / (seq_t)2;
        idx_t p = (idx_t)pk;
        if (p > n) break;

        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    return dp[n];
}
