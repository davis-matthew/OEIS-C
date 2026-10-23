/*
 * A134522  a(n) = 2^n + ceiling(n/2).
 * https://oeis.org/A134522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + ceil(n/2)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t two_pow = pow2(n);          /* 2^n */
    seq_t half     = (seq_t)n / 2;    /* floor(n/2) in seq_t */
    if (n & 1)                       /* add 1 if n is odd */
        half += (seq_t)1;
    return two_pow + half;            /* 2^n + ceil(n/2) */
}
