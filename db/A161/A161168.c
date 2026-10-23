/*
 * A161168  a(n) = 2^n + 4^n.
 * https://oeis.org/A161168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + 4^n Recurrence: a(0)=2, a(1)=6,
 *            a(n)=6*a(n-1)-8*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val2 = 1, val4 = 1;
    idx_t i = n;
    while (i--) {
        val2 *= (seq_t)2;
        val4 *= (seq_t)4;
    }
    return val2 + val4;
}
