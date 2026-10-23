/*
 * A128533  a(n) = F(n)*L(n+2) where F=Fibonacci and L=Lucas numbers.
 * https://oeis.org/A128533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n)*L(n+2) (recurrence: a(0) = 0, a(1) = 4, a(2) = 7,
 *            a(n) = 2*a(n‑1) + 2*a(n‑2) – a(n‑3) for n ≥ 3)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)7;

    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)4;
    seq_t a2 = (seq_t)7;
    seq_t two = (seq_t)2;
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = two * a2 + two * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
