/*
 * A013741  a(n) = 7^(3*n + 2).
 * https://oeis.org/A013741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^(3*n + 2)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 14,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute exponent = 3*n + 2 and evaluate 7^(exponent) */
    idx_t exp = 3 * n + 2;
    return pow_uint(7, exp);
}
