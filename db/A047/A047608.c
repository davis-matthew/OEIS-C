/*
 * A047608  Numbers that are congruent to {4, 5} mod 8.
 * https://oeis.org/A047608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - 3*((n-1) mod 2)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n * 4;                 /* 4*n */
    if (((n - 1) & 1ULL) != 0)                /* (n-1) mod 2 == 1? */
        val -= 3;                             /* subtract 3 if odd */
    return val;
}
