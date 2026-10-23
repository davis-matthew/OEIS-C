/*
 * A100314  Number of 2 X n 0-1 matrices avoiding simultaneously the right
 *          angled numbered polyomino patterns (ranpp) (00;1), (01;0), (10;0)
 *          and (01;1).
 * https://oeis.org/A100314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + 2*n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^n via the utility function, then add 2*n */
    seq_t pow = pow2(n);          /* 2^n */
    seq_t add = (seq_t)2 * (seq_t)n;   /* 2*n */
    return pow + add;
}
