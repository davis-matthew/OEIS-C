/*
 * A008636  Number of partitions of n into at most 7 parts.
 * https://oeis.org/A008636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 65, u32 n <= 485, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A008636:  Number of partitions of n into at most 7 parts.
    Computed by dynamic programming on the generating function
    1/((1-x)(1-x^2)...(1-x^7)).  Each a(n) is obtained in O(7n)
    arithmetic on the type seq_t.  The routine is re‑computable on
    every call, so it is valid for any access pattern. */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;        /* array of partition numbers */
    static idx_t dp_size = 0;      /* current allocated size */

    /* Allocate or enlarge the dp array to at least n+1 elements. */
    if (dp_size < n + 1) {
        seq_t *new_dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_dp) return (seq_t)0;          /* out of memory; should not happen */
        memset(new_dp, 0, (n + 1) * sizeof(seq_t));
        free(dp);
        dp     = new_dp;
        dp_size = n + 1;
    } else {
        /* For an existing array, clear the entries that will be recomputed. */
        if (n >= 1)
            memset(dp + 1, 0, n * sizeof(seq_t));
    }

    /* Base case: there is exactly one way to partition 0. */
    dp[0] = (seq_t)1;

    /* Dynamic programming on parts 1..7. */
    for (idx_t part = 1; part <= 7; ++part) {
        for (idx_t sum = part; sum <= n; ++sum) {
            dp[sum] += dp[sum - part];
        }
    }

    return dp[n];
}
