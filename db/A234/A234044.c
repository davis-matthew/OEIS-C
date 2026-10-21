/*
 * A234044  Period 7: repeat [2, -2, 1, 0, 0, 1, -2].
 * https://oeis.org/A234044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n+7)=a(n); a(0)=2, a(1)=-2, a(2)=1, a(3)=0, a(4)=0, a(5)=1,
 *            a(6)=-2
 * Range:     i8 n <= 92, i16 n <= 92, i32 n <= 92, i64 n <= 92, i128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t pat[7] = { 2, -2, 1, 0, 0, 1, -2 };
    return pat[(size_t)(n % 7u)];
}
