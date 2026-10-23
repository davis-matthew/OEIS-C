/*
 * A163659  L.g.f.: Sum_{n>=1} a(n)*x^n/n = log(S(x)/x) where S(x) is the g.f.
 *          of Stern's diatomic series (A002487).
 * https://oeis.org/A163659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = (2^(v2(n)+1)-1) if n contains a factor 2, multiplied by
 *            (-2) if n contains a factor 3; otherwise a(n)=1. Here v2(n) is
 *            the exponent of 2 in the prime factorisation of n
 * Range:     i8 n <= 127, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t tmp = n;
    /* count exponent of 2 */
    idx_t e2 = 0;
    while ((tmp & 1ULL) == 0 && tmp > 0) {
        e2++;
        tmp >>= 1;
    }
    if (e2 > 0) {
        seq_t p = pow2((idx_t)(e2 + 1));   /* 2^(e2+1) */
        result *= (p - 1);
    }
    /* check for factor 3 (multiplicative factor -2 if present) */
    if (tmp % 3ULL == 0) {
        result *= -2;
    }
    return result;
}
