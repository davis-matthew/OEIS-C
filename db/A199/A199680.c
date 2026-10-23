/*
 * A199680  a(n) = (11*9^n+1)/2.
 * https://oeis.org/A199680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*9^n+1)/2 Recurrence: a(0)=6, a(n)=9*a(n-1)-4
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)6;

    seq_t res = (seq_t)6;
    for (idx_t i = 1; i <= n; ++i)
    {
        res = res * (seq_t)9 - (seq_t)4;
    }
    return res;
}
