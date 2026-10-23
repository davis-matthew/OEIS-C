/*
 * A160450  Expansion of
 *          (1-34*x+276*x^2-584*x^3)/((1-3*x)*(1-4*x)*(1-8*x)*(1-24*x)).
 * https://oeis.org/A160450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-1) + 2*4^(n-1) + 8^(n-1) + 24^(n-1) (with a(0) = 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* special case for the offset 0 */
    if (n == 0)
        return (seq_t)1;

    /* compute each power using exponentiation by squaring */
    seq_t p3   = pow_uint((uint64_t)3, n - 1);
    seq_t p4   = pow_uint((uint64_t)4, n - 1);
    seq_t p8   = pow_uint((uint64_t)8, n - 1);
    seq_t p24  = pow_uint((uint64_t)24, n - 1);

    /* combine the terms */
    return p3 + (seq_t)2 * p4 + p8 + p24;
}
