/*
 * A175824  Maximum unsigned integer that can be stored in n bytes.
 * https://oeis.org/A175824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(8*n) - 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t val = pow_uint((uint64_t)2, 8 * n);
    val -= (seq_t)1;
    return val;
}
