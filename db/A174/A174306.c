/*
 * A174306  Values of the family of polynomials y = x^n - x - 1 at point x =
 *          discriminant of y.
 * https://oeis.org/A174306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (disc(x^n - x - 1))^n - disc(x^n - x - 1) - 1 * where
 *            disc(f) is the discriminant of the monic polynomial * f(x) = x^n
 *            - x - 1 * The discriminant has the closed form * disc(f) =
 *            (-1)^{n(n-1)/2 + 1} * ( (-1)^n * n^n + (n-1)^{n-1} ) * All
 *            arithmetic is performed with seq_t
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 3, i64 n <= 5, i128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res = res * base;
    return res;
}

seq_t a(idx_t n)
{
    /* handle the special first term */
    if (n == 1)
        return (seq_t)-1;

    /* sign of the discriminant: (-1)^{n(n-1)/2 + 1} */
    idx_t halfprod = n * (n - 1) / 2;
    int  sign1_int = ((halfprod + 1) & 1) == 0 ? 1 : -1;
    seq_t sign1 = (seq_t)sign1_int;

    /* (-1)^n */
    int  s2_int = (n & 1) == 0 ? 1 : -1;
    seq_t s2 = (seq_t)s2_int;

    /* n^n */
    seq_t term1 = pow_seq((seq_t)n, n);

    /* (n-1)^(n-1) */
    seq_t term2 = pow_seq((seq_t)(n - 1), n - 1);

    /* inner expression */
    seq_t inner = s2 * term1 + term2;

    /* discriminant */
    seq_t disc = sign1 * inner;

    /* disc^n */
    seq_t abs_disc = disc >= (seq_t)0 ? disc : -disc;
    seq_t abs_disc_pow = pow_seq(abs_disc, n);
    seq_t disc_pow;
    if (disc < (seq_t)0 && (n & 1) == 1)
        disc_pow = -abs_disc_pow;
    else
        disc_pow = abs_disc_pow;

    /* final term */
    seq_t result = disc_pow - disc - (seq_t)1;
    return result;
}
