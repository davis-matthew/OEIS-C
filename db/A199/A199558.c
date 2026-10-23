/*
 * A199558  a(n) = 11*8^n+1.
 * https://oeis.org/A199558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*8^n+1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute 8^n exactly and multiply by 11, then add 1. */
    seq_t pow8 = pow_uint(8, n);
    return pow8 * (seq_t)11 + (seq_t)1;
}
