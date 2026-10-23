/*
 * A016923  a(n) = (6*n + 1)^3.
 * https://oeis.org/A016923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 1)^3
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 270, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = ((seq_t)6 * (seq_t)n + (seq_t)1);
    return x * x * x;
}
