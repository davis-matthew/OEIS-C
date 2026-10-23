/*
 * A011905  a(n) = floor( n*(n-1)*(n-2)/23 ).
 * https://oeis.org/A011905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)/23)
 * Range:     u8 n <= 19, u16 n <= 115, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) {
        return 0;
    }
    uint64_t nn = (uint64_t)n;
    uint64_t prod = nn * (nn - 1) * (nn - 2);
    return (seq_t)(prod / 23);
}
