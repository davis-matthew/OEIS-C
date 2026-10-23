/*
 * A128536  a(n) = numerator of r(n): r(n) is such that, for every positive
 *          integer n, the continued fraction (of rational terms)
 *          [r(1);r(2),...,r(n)] equals n(n+1)/2, the n-th triangular number.
 * https://oeis.org/A128536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, sign
 * Formula:   a(n) is the numerator of r(n), where r(1) = 1 r(2) = 1/2 r(3) =
 *            -10/3 and for n >= 4, r(n) = -(2n-1)(2n-3) / ( n (n-2) r(n-1) ).
 *            After reducing the fraction to lowest terms, the numerator is
 *            returned
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 15, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle offset 1 and the first two values directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;

    /* r1 = 1/1, r2 = 1/2 */
    seq_t N_prev = (seq_t)1;   /* numerator of r(n-1) */
    seq_t D_prev = (seq_t)1;   /* denominator of r(n-1) */

    /* Compute r2 */
    seq_t N_cur = (seq_t)1;
    seq_t D_cur = (seq_t)2;
    if (n == 2) return N_cur;   /* already handled, but kept for completeness */

    /* Compute r3 directly (recurrence is not valid for n==3). */
    N_cur = -(seq_t)10;
    D_cur = (seq_t)3;
    if (n == 3) return N_cur;

    /* Iterate from n = 4 up to the requested n, keeping the last
       fraction r(n) as N_cur/D_cur. */
    for (idx_t i = 4; i <= n; ++i)
    {
        /* Compute numerator and denominator of r(i) using r(i-1) = N_cur/D_cur. */
        seq_t num = -(seq_t)(2 * i - 1) * (seq_t)(2 * i - 3) * D_cur;
        seq_t den = (seq_t)i * (seq_t)(i - 2) * N_cur;

        /* Reduce the fraction to lowest terms. */
        seq_t absnum = num >= 0 ? num : -num;
        seq_t absden = den >= 0 ? den : -den;
        seq_t g = gcd(absnum, absden);
        num /= g;
        den /= g;

        /* Keep denominator positive. */
        if (den < 0) {
            den = -den;
            num = -num;
        }

        /* Update current fraction for the next iteration. */
        N_cur = num;
        D_cur = den;
    }

    return N_cur;   /* numerator of r(n) in lowest terms */
}
