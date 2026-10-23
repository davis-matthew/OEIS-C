/*
 * A032299  "EFJ" (unordered, size, labeled) transform of 1,2,3,4,...
 * https://oeis.org/A032299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Product_{m>0} (1 + x^m/(m-1)!)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 25, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A032299:
   a(0)=1; for n>0,
   a(n) = sum over all partitions of [n] into blocks of distinct sizes
   of the product of the block sizes.
   This is equivalent to the coefficient of x^n in
   ∏_{m≥1} (1 + x^m/(m-1)!) multiplied by n!. */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* Allocate array for dynamic programming, initialized to 0. */
    size_t sz = (size_t)(n + 1);
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp) {
        /* Memory allocation failed; return 0 as a safe fallback. */
        return (seq_t)0;
    }
    dp[0] = (seq_t)1;

    /* DP over block sizes m = 1 .. n.
       For each m we add the option of using a block of size m
       exactly once.  We update dp[t] for all t ≥ m using
       the binomial coefficient binom(t, m).  The update rule is
       dp[t] += binom(t, m) * m * dp[t - m].  The binomial
       coefficient is updated incrementally as t decreases.   */
    for (idx_t m = 1; m <= n; ++m) {
        /* Compute binom(n, m) in seq_t. */
        seq_t binom = (seq_t)1;
        for (idx_t i = 0; i < m; ++i) {
            binom *= (seq_t)(n - i);
            binom /= (seq_t)(i + 1);
        }

        for (idx_t t = n; t >= m; --t) {
            seq_t term = binom * (seq_t)m * dp[t - m];
            dp[t] += term;

            /* Update binom for the next smaller t:  binom(t-1, m) = binom(t, m) * (t-m)/t. */
            if (t - 1 >= m) {
                binom = binom * (seq_t)(t - m);
                binom = binom / (seq_t)t;
            }
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
