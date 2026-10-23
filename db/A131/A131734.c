/*
 * A131734  Period 6: repeat [0, 1, 0, 1, 0, -1].
 * https://oeis.org/A131734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-6); a(0)=0,a(1)=1,a(2)=0,a(3)=1,a(4)=0,a(5)=-1
 * Range:     i8 n <= 118, i16 n <= 118, i32 n <= 118, i64 n <= 118,
 *            i128 n <= 118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t tbl[6] = { (seq_t)0, (seq_t)1, (seq_t)0,
                                 (seq_t)1, (seq_t)0, (seq_t)-1 };
    return tbl[(size_t)(n % 6)];
}
