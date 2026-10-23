/*
 * A157765  Expansion of 2*(1 - x) / (1 - 10*x - 7*x^2).
 * https://oeis.org/A157765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + 7*a(n-2), * a(0) = 2, a(1) = 18
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)18;

    seq_t a0 = (seq_t)2;
    seq_t a1 = (seq_t)18;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = (seq_t)10 * a1 + (seq_t)7 * a0;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
