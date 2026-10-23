/*
 * A139641  a(n) = binomial(n+4, 4)*7^n.
 * https://oeis.org/A139641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+4, 4) * 7^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C(n+4, 4) */
    seq_t bin = binomial(n + 4, 4);

    /* Compute 7^n */
    seq_t pow7 = pow_uint(7, n);

    /* Return the product */
    return bin * pow7;
}
