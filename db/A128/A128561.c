/*
 * A128561  a(n) = denominator of r(n): r(n) is such that the continued
 *          fraction (of rational terms) [r(1);r(2),...,r(n)] = n^2, for every
 *          positive integer n.
 * https://oeis.org/A128561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A128561: a(n) = denominator of r(n) where
   r(n) = -16*(n-1)*(n-2)/((2n-1)*(2n-5)*r(n-1)),
   with r(1)=1, r(2)=1/3, r(3)=-24/5. */
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;

    /* r(3) = -24/5   (numerator 24, denominator 5). */
    seq_t num_prev = (seq_t)24;   /* absolute numerator of r(n-1) */
    seq_t den_prev = (seq_t)5;    /* absolute denominator of r(n-1) */

    /* Compute r(4) ... r(n) iteratively. */
    for (idx_t k = 4; k <= n; ++k)
    {
        /* Calculate new numerator:
           new_num_abs = 16 * (k-1) * (k-2) * den_prev. */
        seq_t new_num = den_prev;
        new_num = new_num * (seq_t)(k - 1);
        new_num = new_num * (seq_t)(k - 2);
        new_num = new_num * (seq_t)16;

        /* Calculate new denominator:
           new_den_abs = (2k-1) * (2k-5) * num_prev. */
        seq_t new_den = num_prev;
        new_den = new_den * (seq_t)(2 * k - 1);
        new_den = new_den * (seq_t)(2 * k - 5);

        /* Reduce the fraction to lowest terms. */
        seq_t g = seq_gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;

        /* Update the previous fraction for next iteration. */
        num_prev = new_num;
        den_prev = new_den;
    }

    /* For n ≥ 4 the denominator of r(n) is the desired a(n). */
    return den_prev;
}
