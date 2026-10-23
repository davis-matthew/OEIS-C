/*
 * A135440  a(n) = a(n-1) + 2a(n-2).
 * https://oeis.org/A135440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n - 2*(-1)^n (offset 0)
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 62, i128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t val = pow2(n);

    /* Adjust for (-1)^n term */
    if ((n & 1) == 0)   /* even n: (-1)^n = 1  -> subtract 2 */
        val -= (seq_t)2;
    else                /* odd n:  (-1)^n = -1 -> add 2  */
        val += (seq_t)2;

    return val;
}
