/*
 * A172472  a(n) = floor(sqrt(2*n^3)).
 * https://oeis.org/A172472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(2*n^3))
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 780, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n * n * n * 2;
    return floor_sqrt(val);
}
