/*
 * A217274  a(n) = 7*a(n-1) - 14*a(n-2) + 7*a(n-3) with a(0)=0, a(1)=1,
 *          a(2)=7.
 * https://oeis.org/A217274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 14*a(n-2) + 7*a(n-3), a(0) = 0, a(1) = 1, a(2)
 *            = 7
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)7;

    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)7;
    seq_t a_next;

    for (idx_t i = 3; i <= n; ++i)
    {
        a_next = (seq_t)7 * a2 - (seq_t)14 * a1 + (seq_t)7 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a_next;
    }

    return a2;
}
