/*
 * A196280  a(n) = binomial(n+9, 9)*8^n.
 * https://oeis.org/A196280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+9,9) * 8^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the provided binomial() and pow_uint() helpers. */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute 8^n */
    seq_t p8 = pow_uint((uint64_t)8, n);

    /* Compute C(n+9, 9) */
    seq_t c = binomial(n + 9, 9);

    /* Return the product */
    return c * p8;
}
