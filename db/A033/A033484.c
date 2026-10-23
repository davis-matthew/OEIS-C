/*
 * A033484  a(n) = 3*2^n - 2.
 * https://oeis.org/A033484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n - 2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t p = pow2(n);

    /* Multiply by 3 and subtract 2 */
    p = p * (seq_t)3;
    p -= (seq_t)2;

    return p;
}
