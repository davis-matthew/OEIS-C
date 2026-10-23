/*
 * A171638  Denominator of 1/(n-2)^2 - 1/(n+2)^2.
 * https://oeis.org/A171638
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of 1/(n-2)^2 - 1/(n+2)^2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2.
       For n = 2 the expression has a pole, so we return 0. */
    if (n <= 2)
        return (seq_t)0;

    seq_t sn = (seq_t)n;          /* cast to seq_t once */
    seq_t a_minus = sn - (seq_t)2;/* n-2  */
    seq_t a_plus  = sn + (seq_t)2;/* n+2  */
    seq_t num = (seq_t)8 * sn;    /* numerator 8n */

    /* Full denominator before reduction */
    seq_t denom_full = (a_minus * a_minus) * (a_plus * a_plus);

    /* Reduce fraction by eliminating common factors */
    seq_t g = gcd(num, denom_full);
    return denom_full / g;
}
