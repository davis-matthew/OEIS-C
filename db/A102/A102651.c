/*
 * A102651  a(n) = 4 * floor(23*2^n/15).
 * https://oeis.org/A102651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = 4 * floor(23*2^n/15)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t p = pow2(n);                  /* 2^n */
    seq_t v = (seq_t)23 * p / 15;       /* floor(23*2^n/15) */
    return v * 4;                       /* final result */
}
