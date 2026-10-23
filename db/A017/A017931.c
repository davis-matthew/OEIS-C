/*
 * A017931  Expansion of 1/((1-3x)(1-6x)(1-7x)).
 * https://oeis.org/A017931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=16, a(2)=175; for n>2,
 *            a(n)=16*a(n-1)-81*a(n-2)+126*a(n-3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)175;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)16;
    seq_t a2 = (seq_t)175;
    seq_t a3;
    seq_t c16 = (seq_t)16;
    seq_t c81 = (seq_t)81;
    seq_t c126 = (seq_t)126;

    for (idx_t i = 3; i <= n; ++i)
    {
        a3 = c16 * a2 - c81 * a1 + c126 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
