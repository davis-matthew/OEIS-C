/*
 * A074768  Related to series for Laplace limit constant.
 * https://oeis.org/A074768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n-1} (-1)^k * (2*n-k) * C(n-1,k) * (4*n)^k * n!
 *            / (k+2)!
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t and idx_t are provided by the build system. */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;

    /* n! */
    seq_t fact_n = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact_n *= (seq_t)i;

    /* factorial for (k+2)!; initially (0+2)! = 2! */
    seq_t fact_k2 = 2;

    /* binomial C(n-1,0) */
    seq_t binom = 1;

    /* (4*n)^k, initially 1 for k=0 */
    seq_t pow_val = 1;
    seq_t four_n = (seq_t)(4 * n);

    seq_t sum = 0;

    for (idx_t k = 0; k <= n - 1; ++k) {
        /* temp = n! * (2*n - k) / (k+2)!  (exact division) */
        seq_t temp = fact_n * (seq_t)(2 * n - k);
        temp = temp / fact_k2;                 /* exact */

        /* term = temp * binom * (4*n)^k * sign */
        seq_t term = temp * binom * pow_val;
        if (k & 1)
            sum -= term;
        else
            sum += term;

        /* prepare for next k (if any) */
        if (k < n - 1) {
            /* update factorial (k+2)! -> (k+3)! */
            fact_k2 *= (seq_t)(k + 3);

            /* update binomial C(n-1,k+1) */
            seq_t num = (seq_t)(n - 1 - k);
            seq_t den = (seq_t)(k + 1);
            binom = (binom * num) / den;

            /* update power (4*n)^(k+1) */
            pow_val *= four_n;
        }
    }
    return sum;
}
