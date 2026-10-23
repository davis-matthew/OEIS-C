/*
 * A022269  a(n) = n*(11*n+1)/2.
 * https://oeis.org/A022269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(11*n+1)/2
 * Range:     u8 n <= 6, u16 n <= 109, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    uint64_t temp = (uint64_t)nn * ((uint64_t)nn * 11 + 1);
    temp /= 2;
    return (seq_t)temp;
}
