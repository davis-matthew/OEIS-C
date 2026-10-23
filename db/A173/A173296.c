/*
 * A173296  Numerators of the inverse binomial transform of the Leibniz series
 *          for Pi/4.
 * https://oeis.org/A173296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, sign
 * Formula:   a(n) = (-1)^n * Sum_{k=0}^{n} C(n,k)/(2k+1) The sum is the
 *            integral I_n = ∫_0^1 (1+x^2)^n dx, which satisfies I_0 = 1 and
 *            I_n = (2^n + 2n*I_{n-1})/(2n+1). The numerator after simplifying
 *            the fraction is returned, with sign (-1)^n
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

extern seq_t a(idx_t n)
{
    seq_t num = 1;          /* numerator of I_0 */
    seq_t den = 1;          /* denominator of I_0 */
    seq_t pow2 = 1;         /* will hold 2^k */

    for (idx_t k = 1; k <= n; ++k) {
        pow2 <<= 1;                          /* pow2 = 2^k */

        seq_t term1 = pow2 * den;            /* 2^k * previous denominator */
        seq_t term2 = (seq_t)(2 * k) * num;  /* 2k * previous numerator */
        seq_t new_num = term1 + term2;       /* numerator before reduction */
        seq_t new_den = den * (seq_t)(2 * k + 1); /* denominator before reduction */

        seq_t g = gcd(new_num, new_den);    /* reduce fraction */
        new_num /= g;
        new_den /= g;

        num = new_num;
        den = new_den;
    }

    if ((n & 1) != 0) { /* apply (-1)^n */
        num = -num;
    }

    return num;
}
