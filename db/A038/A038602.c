/*
 * A038602  One half of convolution of central binomial coefficients
 *          A000984(n) with A000984(n+2), n >= 0.
 * https://oeis.org/A038602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(2*n+3) - (3*n+5)*C(n+1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* exponent for 2 */
    idx_t exp = 2*n + 3;
    seq_t powterm = pow2(exp);

    /* Catalan number C(n+1) */
    seq_t cat = catalan(n + 1);

    /* (3*n + 5) as seq_t */
    seq_t mult = (seq_t)3 * (seq_t)n + 5;

    /* subtraction gives the required value */
    return powterm - cat * mult;
}
