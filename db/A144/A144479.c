/*
 * A144479  a(0)=1, a(1)=3, a(n) = 8*a(n-1) - a(n-2).
 * https://oeis.org/A144479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(n)=8*a(n-1)-a(n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t prev = (seq_t)1;
    seq_t cur  = (seq_t)3;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = (cur << 3) - prev; /* 8*cur - prev */
        prev = cur;
        cur  = next;
    }
    return cur;
}
