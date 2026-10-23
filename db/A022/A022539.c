/*
 * A022539  Nexus numbers (n+1)^23 - n^23.
 * https://oeis.org/A022539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^23 - n^23
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 5, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t base1 = nn + 1;
    seq_t base2 = nn;
    seq_t p1 = 1;
    seq_t p2 = 1;
    for (size_t i = 0; i < 23; ++i) {
        p1 *= base1;
        p2 *= base2;
    }
    return p1 - p2;
}
