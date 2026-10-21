/*
 * A202304  a(n) = floor(sqrt(3*n)).
 * https://oeis.org/A202304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(3*n))
 * Range:     u8 n <= 21, u16 n <= 5114, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    return floor_sqrt((seq_t)n * 3);
}
