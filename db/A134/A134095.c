/*
 * A134095  Expansion of e.g.f. A(x) = 1/(1 - LambertW(-x)^2).
 * https://oeis.org/A134095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..n} C(n,k) · (n−k)^k · k^(n−k) Equivalent to: a(n)
 *            = n! · ∑_{k=0..n} (−1)^(n−k) · n^k / k! The implementation uses
 *            the latter form to avoid explicitly computing binomial
 *            coefficients and powers
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the n‑th term of sequence A134095. */
seq_t a(idx_t n)
{
    /* compute n! (factorial of n) */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    /* sums of the terms with positive and negative sign
       respectively (the final result is the difference) */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    /* prod holds n! / k! at the current k */
    seq_t prod = fact;            /* for k = 0  →  n! / 0! = n!      */
    seq_t pow_n = 1;              /* for k = 0  →  n^0 = 1          */
    bool positive = ((n & 1) == 0);   /* sign of the term for k = 0:
                                         (−1)^(n−k)  →  (−1)^n
                                         even n  →  positive  */
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = prod * pow_n;
        if (positive)
            sum_pos += term;
        else
            sum_neg += term;

        if (k == n)
            break;

        /* prepare values for k+1 */
        pow_n *= (seq_t)n;          /* n^(k+1) */
        prod /= (k + 1);           /* n! / (k+1)!    exact division */
        positive = !positive;      /* flip sign (−1)^(n−(k+1)) */
    }

    /* final result is the difference of the two sums */
    return sum_pos - sum_neg;
}
