/*
 * A218991  Power floor sequence of 3+sqrt(10).
 * https://oeis.org/A218991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 5*a(n-2) - a(n-3) with a(0)=6, a(1)=36,
 *            a(2)=221
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)6;
    if (n == 1) return (seq_t)36;
    if (n == 2) return (seq_t)221;

    seq_t t0 = (seq_t)6;
    seq_t t1 = (seq_t)36;
    seq_t t2 = (seq_t)221;
    seq_t t3;

    for (idx_t i = 3; i <= n; ++i) {
        t3 = t2 * 7;
        t3 -= t1 * 5;
        t3 -= t0;
        t0 = t1;
        t1 = t2;
        t2 = t3;
    }
    return t2;
}
