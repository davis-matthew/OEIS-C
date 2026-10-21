/*
 * A083299  a(n) = (4*6^n + (-4)^n)/5.
 * https://oeis.org/A083299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*6^n + (-4)^n)/5 Equivalent linear recurrence: a(0) = 1
 *            a(1) = 4 a(n) = 2*a(n-1) + 24*a(n-2) for n >= 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t prevprev = (seq_t)1;  /* a(0) */
    seq_t prev     = (seq_t)4;  /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)2 * prev + (seq_t)24 * prevprev;
        prevprev = prev;
        prev = cur;
    }
    return cur;
}
