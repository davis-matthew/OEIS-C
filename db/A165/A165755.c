/*
 * A165755  a(n) = (5-3*5^n)/2.
 * https://oeis.org/A165755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (5-3*5^n)/2; recurrence: a(0)=1, a(n)=5*a(n-1)-10
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 13, i64 n <= 26, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t val = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        val = (seq_t)5 * val - (seq_t)10;
    }
    return val;
}
