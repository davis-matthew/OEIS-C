/*
 * A153893  a(n) = 3*2^n - 1.
 * https://oeis.org/A153893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n - 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t v = pow2(n);        /* 2^n */
    v = v * (seq_t)3;         /* 3*2^n */
    return v - (seq_t)1;      /* 3*2^n - 1 */
}
