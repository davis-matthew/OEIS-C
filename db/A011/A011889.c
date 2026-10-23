/*
 * A011889  a(n) = floor(n*(n-1)*(n-2)/7).
 * https://oeis.org/A011889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)/7)
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn - 1);
    prod *= (nn - 2);
    return prod / 7;
}
