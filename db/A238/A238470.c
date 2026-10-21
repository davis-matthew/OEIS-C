/*
 * A238470  Period 7: repeat [0, 0, 1, 0, 0, -1, 0].
 * https://oeis.org/A238470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = periodic sequence with period 7: [0, 0, 1, 0, 0, -1, 0]
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n+7) = a(n) for all n >= 0, with a(0)=0, a(1)=0, a(2)=1, a(3)=0, a(4)=0, a(5)=-1, a(6)=0. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    static const seq_t period[7] = {0, 0, 1, 0, 0, -1, 0};
    return period[(size_t)(n % 7)];
}
