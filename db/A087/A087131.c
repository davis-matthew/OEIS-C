/*
 * A087131  a(n) = 2^n*Lucas(n), where Lucas = A000032.
 * https://oeis.org/A087131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n * Lucas(n) Recurrence: a(0) = 2, a(1) = 2 a(n) =
 *            2*a(n-1) + 4*a(n-2) for n ≥ 2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)2;

    seq_t a0 = (seq_t)2;   /* a(0) */
    seq_t a1 = (seq_t)2;   /* a(1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i)
    {
        next = (seq_t)(2 * a1 + 4 * a0);   /* recurrence */
        a0 = a1;
        a1 = next;
    }
    return a1;
}
