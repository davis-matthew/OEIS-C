/*
 * A168415  a(n) = 2^n + 7.
 * https://oeis.org/A168415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + 7
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Computes the n-th term of OEIS sequence A168415: a(n) = 2^n + 7. */
seq_t a(idx_t n)
{
    seq_t val = pow2(n);   /* 2^n */
    val += (seq_t)7;       /* add constant 7 */
    return val;
}
