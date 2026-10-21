/*
 * A064986  Number of partitions of n into factorial parts (0! not allowed).
 * https://oeis.org/A064986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of partitions of n into factorial parts (1! = 1,
 *            2! = 2, 3! = 6, …). Computation: dynamic programming over the
 *            factorials
 * Range:     u8 n <= 59, u16 n <= 409, u32 n <= 6207, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, free */

seq_t a(idx_t n)
{
    /* Allocate or enlarge the DP array as needed */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;
    if (dp_size <= n) {
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (dp) free(dp);
        dp = new_dp;
        dp_size = new_size;
    }

    /* Initialise DP table */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* Compute all factorials ≤ n (there are at most 8 for n ≤ 10000) */
    idx_t facts[20];
    idx_t fact_cnt = 0;
    idx_t f = 1;
    for (idx_t k = 1; ; ++k) {
        if (k > 1) f *= k;   /* f = k! */
        if (f > n) break;
        facts[fact_cnt++] = f;
    }

    /* Dynamic programming: for each factorial add partitions that use it */
    for (idx_t fi = 0; fi < fact_cnt; ++fi) {
        idx_t part = facts[fi];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    return dp[n];
}
