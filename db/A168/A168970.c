/*
 * A168970  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A168970 – Number of reduced words of length n in the Coxeter group
 *  with generators S_i (i=1..5) satisfying S_i^2 = (S_i S_j)^3 = 1.
 *
 *  The sequence follows a linear recurrence of order 23 whose generating
 *  function is
 *
 *        t^23 + 2*t^22 + 2*t^21 + ... + 2*t + 1
 *    ------------------------------------------
 *        6*t^23 - 3*t^22 - 3*t^21 - ... - 3*t + 1
 *
 *  From this the recurrence is
 *
 *      a(0) = 1
 *      a(n) = 5 * 4^(n-1)           for 1 ≤ n ≤ 22
 *      a(23) = 3 * Σ_{k=1}^{22} a(k) - 6*a(0) + 1
 *      a(n)  = 3 * Σ_{k=1}^{22} a(n-k) - 6*a(n-23)   for n ≥ 24
 *
 *  The implementation below computes a(n) in O(n) time and O(n) space
 *  using only permitted operations.
 */

#include <stdlib.h>   /* for malloc, free */

/* Global: no external linkage shall be used.  */
seq_t a(idx_t n)
{
    /* Special case n == 0. */
    if (n == 0)
        return (seq_t)1;

    /* Allocate an array of length (n+1) to hold the sequence values. */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;   /* OOM; behavior undefined but safe fallback */

    /* Base term. */
    dp[0] = (seq_t)1;

    /* 1 ≤ n ≤ 22 : a(n) = 5 * 4^(n-1).  Compute iteratively. */
    if (n >= 1) {
        seq_t val = (seq_t)5;
        for (idx_t i = 1; i <= n && i <= 22; ++i) {
            dp[i] = val;
            val = val * (seq_t)4;
        }
    }

    /* n = 23 : special recurrence with +1. */
    if (n >= 23) {
        seq_t sum = (seq_t)0;
        for (idx_t i = 1; i <= 22; ++i) sum += dp[i];
        dp[23] = sum * (seq_t)3 - dp[0] * (seq_t)6 + (seq_t)1;
    }

    /* n ≥ 24 : homogeneous linear recurrence. */
    if (n >= 24) {
        for (idx_t i = 24; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 22; ++k) sum += dp[i - k];
            dp[i] = sum * (seq_t)3 - dp[i - 23] * (seq_t)6;
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
