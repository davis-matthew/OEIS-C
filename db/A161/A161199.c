/*
 * A161199  Numerators in expansion of (1-x)^(-5/2).
 * https://oeis.org/A161199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = numerator(((3 + 8*n + 4*n^2)/3)*binomial(2*n,n)/(4^n))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   binomial, gcd, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow4.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* compute poly = 3 + 8*n + 4*n^2 in seq_t type */
    seq_t tn = (seq_t)n;
    seq_t poly = (seq_t)3 + (seq_t)8 * tn + (seq_t)4 * tn * tn;

    /* binomial(2*n,n) */
    seq_t bin = binomial(2ULL * n, n);

    /* denominator = 3 * 4^n */
    seq_t den = (seq_t)3 * pow4(n);

    /* first cancel factors between bin and den */
    seq_t g = gcd(bin, den);
    bin /= g;
    den /= g;

    /* cancel remaining factors between poly and den */
    g = gcd(poly, den);
    poly /= g;
    den /= g;

    /* final numerator */
    seq_t num = poly * bin;

    /* any remaining common factor with denominator (shouldn't happen) */
    if (den != (seq_t)1) {
        g = gcd(num, den);
        num /= g;
    }

    return num;
}
