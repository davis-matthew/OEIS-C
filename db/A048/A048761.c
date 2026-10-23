/*
 * A048761  Smallest square greater than or equal to n.
 * https://oeis.org/A048761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (ceiling(sqrt(n)))^2
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t s = floor_sqrt(x);
    seq_t sq = s * s;
    if (sq < x) {
        s = s + 1;
    }
    return s * s;
}
