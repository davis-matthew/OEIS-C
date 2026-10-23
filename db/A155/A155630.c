/*
 * A155630  a(n) = 9^n-4^n+1^n.
 * https://oeis.org/A155630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n - 4^n + 1^n Recurrence: a(0) = 1, a(1) = 6, a(n) =
 *            13*a(n-1) - 36*a(n-2) + 24
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;

    seq_t a0 = (seq_t)1; // a(0)
    seq_t a1 = (seq_t)6; // a(1)
    seq_t an = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)13 * a1 - (seq_t)36 * a0 + (seq_t)24;
        a0 = a1;
        a1 = an;
    }
    return an;
}
