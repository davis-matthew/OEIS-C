/*
 * A140405  a(n) = binomial(n+6, 6)*5^n.
 * https://oeis.org/A140405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n + 6, 6) * 5^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute binomial coefficient C(n+6, 6) */
    seq_t bin = binomial(n + 6, 6);

    /* Compute 5^n */
    seq_t pow5 = pow_uint(5, n);

    /* Return the product */
    return bin * pow5;
}
