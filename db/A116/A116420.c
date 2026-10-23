/*
 * A116420  Reduced denominators of 2(2^(1+n)-1)/(1+n)/(2+n).
 * https://oeis.org/A116420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of reduced fraction 2*(2^(n+1)-1) /
 *            ((n+1)*(n+2)) (OEIS A116420)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* compute 2^(n+1) */
    seq_t power = pow_seq((seq_t)2, (int)(n + 1));

    /* numerator = 2*(2^(n+1)-1) */
    seq_t num = (seq_t)2 * (power - (seq_t)1);

    /* denominator = (n+1)*(n+2) */
    seq_t den = (seq_t)(n + 1) * (seq_t)(n + 2);

    /* reduce fraction */
    seq_t g = gcd(num, den);
    return den / g;
}
