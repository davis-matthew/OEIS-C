/*
 * A011861  a(n) = floor(n(n-1)/8).
 * https://oeis.org/A011861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)/8)
 * Range:     u8 n <= 45, u16 n <= 724, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    uint64_t nn = (uint64_t)n;
    uint64_t prod = nn * (nn - 1);
    uint64_t res  = prod / 8;
    return (seq_t)res;
}
