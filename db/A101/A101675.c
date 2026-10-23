/*
 * A101675  Expansion of (1 - x - x^2)/(1 + x^2 + x^4).
 * https://oeis.org/A101675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence of period 6:
   a(0)=1, a(1)=-1, a(2)=-2, a(3)=1, a(4)=1, a(5)=0,
   a(n)=a(n%6) for n>=0. */
#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t vals[6] = { 1, -1, -2, 1, 1, 0 };
    return vals[(size_t)(n % 6)];
}
