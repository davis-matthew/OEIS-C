/*
 * A087204  Period 6: repeat [2, 1, -1, -2, -1, 1].
 * https://oeis.org/A087204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = a(n-1) - a(n-2) with a(0)=2, a(1)=1 (period 6)
 * Range:     i8 n <= 101, i16 n <= 101, i32 n <= 101, i64 n <= 101,
 *            i128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[6] = {
        (seq_t)2,
        (seq_t)1,
        (seq_t)-1,
        (seq_t)-2,
        (seq_t)-1,
        (seq_t)1
    };
    return period[(size_t)(n % 6)];
}
