/*
 * A075135  Numerator of the generalized harmonic number H(n,3,1) described
 *          below.
 * https://oeis.org/A075135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = numerator of the generalized harmonic number H(n,3,1) = 1
 *            + 1/4 + 1/7 + … + 1/(3(n−1)+1) reduced to lowest terms
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* External linkage function */
seq_t a(idx_t n)
{
    seq_t num = (seq_t)0;   /* current numerator */
    seq_t den = (seq_t)1;   /* current denominator */

    for (idx_t i = 0; i < n; ++i) {
        seq_t denom_i = (seq_t)3 * i + (seq_t)1;          /* 3*i + 1 */
        seq_t new_num = num * denom_i + den;              /* num/den + 1/denom_i */
        seq_t new_den = den * denom_i;                    /* common denominator */
        seq_t g = gcd(new_num, new_den);                   /* reduce fraction */
        num = new_num / g;
        den = new_den / g;
    }
    return num;
}
