/*
 * A068980  Number of partitions of n into nonzero tetrahedral numbers.
 * https://oeis.org/A068980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 317, u32 n <= 1661, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A068980 – Number of partitions of n into nonzero tetrahedral numbers.
 *  The implementation uses a classic coin‑change dynamic programming
 *  algorithm with the tetrahedral numbers t_k = binomial(k,3) as the
 *  "coin values".  For each n it recomputes the partition counts from
 *  scratch; this is fast enough for n ≤ 10000 (≈ 4 × 10⁵ elementary
 *  operations per call).  The function works for all supported seq_t
 *  types, relying only on addition and indexing. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The DP array that stores the number of partitions of each sum
       up to n.  It is reallocated as needed. */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* Allocate or enlarge the array to hold all values up to n. */
    if (n + 1 > dp_size) {
        dp_size = n + 1;
        dp = (seq_t *)realloc(dp, dp_size * sizeof(seq_t));
    }

    /* Initialise the array: dp[0] = 1 and all other entries 0. */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* Add each tetrahedral number as a new "coin".  The largest
       tetrahedral number needed fits in an idx_t for n ≤ 10000. */
    for (idx_t k = 3; ; ++k) {
        idx_t t = k * (k - 1) * (k - 2) / 6; /* t = binomial(k,3) */
        if (t > n)
            break;
        for (idx_t i = t; i <= n; ++i)
            dp[i] += dp[i - t];
    }

    return dp[n];
}
