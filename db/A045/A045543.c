/*
 * A045543  6-fold convolution of A000302 (powers of 4); expansion of
 *          1/(1-4*x)^6.
 * https://oeis.org/A045543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+5,5)*4^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 52,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* compute binomial coefficient (n+5 choose 5) */
    seq_t bin = binomial(n + 5, 5);

    /* compute 4^n */
    seq_t pow4 = pow_uint(4, n);

    /* multiply both parts to obtain a(n) */
    return bin * pow4;
}
