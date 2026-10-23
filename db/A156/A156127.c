/*
 * A156127  a(n) = 7*2^n - 3.
 * https://oeis.org/A156127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*2^n - 3
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n);   /* 2^n */
    val = val * 7;         /* 7*2^n */
    val = val - 3;         /* 7*2^n - 3 */
    return val;
}
