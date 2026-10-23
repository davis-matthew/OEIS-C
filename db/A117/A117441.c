/*
 * A117441  Periodic with repeating part {1,1,0,1,-1,0,-1,-1,0,-1,1,0}.
 * https://oeis.org/A117441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-2) - a(n-4), with a(0)=1, a(1)=1, a(2)=0, a(3)=1
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[12] = {
        1,  1,  0,  1, -1,  0,
       -1, -1,  0, -1,  1,  0
    };
    return period[(size_t)(n % 12u)];
}
