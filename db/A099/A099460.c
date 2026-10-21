/*
 * A099460  A Chebyshev transform of A099459 associated to the knot 9_48.
 * https://oeis.org/A099460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=7, a(2)=39, a(3)=203,
   a(n)=7*a(n-1)-11*a(n-2)+7*a(n-3)-a(n-4) for n>=4 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)39;
    if (n == 3) return (seq_t)203;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)7;
    seq_t a2 = (seq_t)39;
    seq_t a3 = (seq_t)203;
    seq_t next;

    for (idx_t i = 4; i <= n; ++i)
    {
        next = (seq_t)7 * a3 - (seq_t)11 * a2 + (seq_t)7 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
