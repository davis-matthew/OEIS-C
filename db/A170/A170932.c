/*
 * A170932  a(n) = binomial(n + 8, 8)*7^n .
 * https://oeis.org/A170932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+8,8)*7^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 15, u128 n <= 35,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C(n+8,8) */
    seq_t bin = binomial(n + 8, 8);

    /* Compute 7^n */
    seq_t pow7 = pow_uint(7, n);

    /* Final result */
    return bin * pow7;
}
