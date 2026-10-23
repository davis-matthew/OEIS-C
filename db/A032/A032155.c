/*
 * A032155  Number of ways to partition n elements into pie slices of
 *          different sizes other than one.
 * https://oeis.org/A032155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 44, u32 n <= 95, u64 n <= 224,
 *            u128 n <= 571, big any n
 * Helpers:   a0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = 1                         (n = 0)
 *  a(n) = Σ_{k≥1} (k-1)! · p_k(n - k(k+3)/2)   (n ≥ 1)
 *
 *  p_k(m) = number of integer partitions of m with parts ≤ k
 *  The value is computed by dynamic programming over k.
 */

#include <stdlib.h>
#include "../utils/a0.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return a0();

    /* Allocate dynamic array for partitions */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return (seq_t)0;        /* allocation failure – unlikely */

    dp[0] = (seq_t)1;                /* p_k(0) = 1 for any k */

    seq_t result = (seq_t)0;
    seq_t fac    = (seq_t)1;          /* (k-1)! : start with k = 1 (0!) */

    for (idx_t k = 1; ; ++k)
    {
        /* shift = k*(k+3)/2  <= n must hold to contribute */
        idx_t shift = k * (k + 3) / 2;
        if (shift > n) break;

        /* Update partition counts to allow parts of size k */
        for (idx_t i = k; i <= n; ++i)
        {
            dp[i] = dp[i] + dp[i - k];
        }

        /* Add contribution of this k to a(n) */
        result = result + fac * dp[n - shift];

        /* Prepare factorial for next k: fac becomes k!   */
        fac = fac * (seq_t)k;
    }

    free(dp);
    return result;
}
