/*
 * A131731  Period 4: repeat [2, -3, 4, -3].
 * https://oeis.org/A131731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((n%4)==0?2:((n%4)==1?-3:((n%4)==2?4:-3)))
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t tbl[4] = { 2, -3, 4, -3 };
    return tbl[(size_t)(n % 4)];
}
