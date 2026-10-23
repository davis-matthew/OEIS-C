/*
 * A197193  a(n) = binomial(n+10, 10)*7^n.
 * https://oeis.org/A197193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+10,10)*7^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 34,
 *            big any n
 * Helpers:   binomial, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Compute C(n+10,10) */
    seq_t bin = binomial(n + 10, 10);
    /* Compute 7^n */
    seq_t pow = pow_uint(7, n);
    /* Return the product */
    return seq_mul(bin, pow);
}
