/*
 * A120994  Numerators of rationals related to John Wallis' product formula
 *          for Pi/2 from his 'Arithmetica infinitorum' from 1659.
 * https://oeis.org/A120994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator(((3/4)*((4^n / C(2n,n))^2)/(2n+1)))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: r(1)=1/1, r(n)=r(n-1)*4n^2/((2n-1)(2n+1)), a(n)=numerator(r(n)) */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n<1, safe fallback */

    seq_t num = 1;    /* numerator of r(n) */
    seq_t den = 1;    /* denominator of r(n) */

    for (idx_t k = 2; k <= n; ++k) {
        /* factors to be multiplied
           num *= 4*k*k
           den *= (2*k-1)*(2*k+1) */
        seq_t fac_num = (seq_t)4 * (seq_t)k * (seq_t)k;
        seq_t fac_den = (seq_t)(2*k-1) * (seq_t)(2*k+1);

        /* cancel common factors between the new numerator and denominator parts */
        seq_t g1 = gcd(fac_num, fac_den);
        if (g1 != 1) {
            fac_num /= g1;
            fac_den /= g1;
        }

        /* cancel common factors between current numerator and new denominator part */
        seq_t g2 = gcd(num, fac_den);
        if (g2 != 1) {
            num /= g2;
            fac_den /= g2;
        }

        /* multiply */
        num *= fac_num;
        den *= fac_den;

        /* final reduction of new fraction */
        seq_t g3 = gcd(num, den);
        if (g3 != 1) {
            num /= g3;
            den /= g3;
        }
    }
    return num;   /* numerator of r(n) */
}
