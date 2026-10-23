/*
 * A174308  Values of the family of polynomials y = Sum_{z = 0..n-1} x^z at
 *          point x = discriminant of y.
 * https://oeis.org/A174308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (D^n – 1)/(D – 1), where D = (–1)^((n–1)(n–2)/2) *
 *            n^(n–2), n ≥ 1
 * Range:     i8 n <= 3, i16 n <= 3, i32 n <= 4, i64 n <= 6, i128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) /* not part of the sequence, but keep defined */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* exponent for n^(n–2)  */
    idx_t exp_n_minus_2 = (idx_t)(n - 2);

    /* base n as seq_t */
    seq_t pow_n = big_pow((seq_t)n, exp_n_minus_2);

    /* sign factor: (n-1)(n-2)/2 mod 2 */
    idx_t parity =
        (((n - 1) * (n - 2)) / 2) & (idx_t)1;

    if (parity)
        pow_n = -pow_n;   /* flip sign if needed */

    seq_t D = pow_n;

    /* handle the trivial case D == 1 (only n == 2) */
    if (D == (seq_t)1)
        return (seq_t)n;

    /* compute a(n) = (D^n – 1)/(D – 1) */
    seq_t pow_D_n   = big_pow(D, n);
    seq_t numerator = pow_D_n - (seq_t)1;
    seq_t denominator = D - (seq_t)1;
    return numerator / denominator;
}
