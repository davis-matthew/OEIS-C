/*
 * A218471  a(n) = n*(7*n-3)/2.
 * https://oeis.org/A218471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(7*n-3)/2
 * Range:     u8 n <= 6, u16 n <= 136, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = nn * (((seq_t)7 * nn) - (seq_t)3);
    return term / 2;
}
