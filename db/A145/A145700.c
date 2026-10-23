/*
 * A145700  Numbers x such that there exists n in N with (x+1)^3-x^3=37*n^2.
 * https://oeis.org/A145700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+2) = 590*a(n+1)-a(n)+294, a(1)=3, a(2)=2068
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)2068;

    seq_t a_prev2 = (seq_t)3;
    seq_t a_prev1 = (seq_t)2068;
    seq_t a_cur   = a_prev1;        /* placeholder */

    for (idx_t i = 3; i <= n; ++i)
    {
        a_cur = (seq_t)590 * a_prev1 - a_prev2 + (seq_t)294;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }
    return a_cur;
}
