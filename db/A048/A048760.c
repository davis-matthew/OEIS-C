/*
 * A048760  Largest square <= n.
 * https://oeis.org/A048760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = floor(sqrt(n))^2
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t s = floor_sqrt((seq_t)n);
    return s * s;
}
