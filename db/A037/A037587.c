/*
 * A037587  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1.
 * https://oeis.org/A037587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*a(n-1) + a(n-2) - 8*a(n-3), a(1) = 3, a(2) = 25, a(3) =
 *            203
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)3;
    if (n == (idx_t)2) return (seq_t)25;
    if (n == (idx_t)3) return (seq_t)203;

    seq_t a1 = (seq_t)3;
    seq_t a2 = (seq_t)25;
    seq_t a3 = (seq_t)203;
    seq_t an = (seq_t)0;

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        an = (seq_t)8 * a3 + a2 - (seq_t)8 * a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
