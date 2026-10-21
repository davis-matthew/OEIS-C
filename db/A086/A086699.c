/*
 * A086699  Number of n X n matrices over GF(2) with rank n-1.
 * https://oeis.org/A086699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{j=0}^{n-2} (2^n−2^j)^2 / (2^(n−1)−2^j)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t num = 1;
    seq_t den = 1;

    seq_t pow2_n   = pow2(n);
    seq_t pow2_n_1 = pow2(n - 1);

    for (idx_t j = 0; j <= n - 2; ++j) {
        seq_t pow2_j = pow2(j);

        seq_t a = pow2_n - pow2_j;     /* 2^n − 2^j                */
        seq_t b = pow2_n_1 - pow2_j;   /* 2^(n−1) − 2^j            */

        /* Square a – may grow, but seq_t is big-capable            */
        seq_t a_sq = a * a;            /* (2^n−2^j)^2              */

        /* Reduce a_sq with current denominator */
        seq_t g = gcd(a_sq, den);
        if (g != 1) {
            a_sq /= g;
            den /= g;
        }

        /* Reduce current numerator with current denominator factor b */
        g = gcd(num, b);
        if (g != 1) {
            num /= g;
            b /= g;
        }

        /* Multiply into the fraction */
        num *= a_sq;
        den *= b;

        /* Final reduction for this step */
        g = gcd(num, den);
        if (g != 1) {
            num /= g;
            den /= g;
        }
    }

    /* After all steps den should be 1; but we simply ignore it */
    return num;
}
