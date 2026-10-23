/*
 * A011901  a(n) = floor( n*(n-1)*(n-2)/19 ).
 * https://oeis.org/A011901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(n-1)*(n-2)/19 )
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1668, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn - 1) * (nn - 2);
    return prod / 19;
}
