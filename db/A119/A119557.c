/*
 * A119557  a(1)=0,a(2)=0,a(3)=1 then a(n)=abs(a(n-1)-a(n-2))-a(n-3).
 * https://oeis.org/A119557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = abs(a(n-1)-a(n-2)) - a(n-3), a(0)=0, a(1)=0, a(2)=1
 * Range:     i8 n <= 99, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t abs_seq(seq_t x)
{
    return x < 0 ? -x : x;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)0;
    seq_t a2 = (seq_t)1;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t term = abs_seq(a1 - a2) - a0;
        a0 = a1;
        a1 = a2;
        a2 = term;
    }
    return a2;
}
