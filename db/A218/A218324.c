/*
 * A218324  Odd heptagonal pyramidal numbers.
 * https://oeis.org/A218324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)*(4*n-3)*(20*n-17)/3
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;
    seq_t t1 = 2 * n0 - 1;
    seq_t t2 = 4 * n0 - 3;
    seq_t t3 = 20 * n0 - 17;

    seq_t prod = t1 * t2;
    prod = prod * t3;
    return prod / 3;
}
