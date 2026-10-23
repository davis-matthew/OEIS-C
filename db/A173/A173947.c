/*
 * A173947  a(n) = numerator of (Zeta(2, 1/4) - Zeta(2, n+1/4)), where Zeta is
 *          the Hurwitz Zeta function.
 * https://oeis.org/A173947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn, easy
 * Formula:   a(n) = numerator of Σ_{k=0}^{n-1} 16/(4k+1)^2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    seq_t num = 0;   /* current numerator of the partial sum */
    seq_t den = 1;   /* current denominator of the partial sum */

    for (idx_t k = 0; k < n; ++k) {
        seq_t kk   = (seq_t)k;
        seq_t d    = (seq_t)4 * kk + 1;   /* 4k+1  */
        seq_t d2   = d * d;               /* (4k+1)^2  */
        seq_t tn   = 16;                  /* numerator of the term  */
        seq_t td   = d2;                  /* denominator of the term  */

        /* Add term tn/td to the running fraction num/den */
        seq_t new_num = num * td + den * tn;
        seq_t new_den = den * td;

        seq_t g = seq_gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return num;   /* the numerator of the reduced fraction */
}
