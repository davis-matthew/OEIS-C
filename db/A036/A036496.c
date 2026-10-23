/*
 * A036496  Number of lines that intersect the first n points on a spiral on a
 *          triangular lattice. The spiral starts at (0,0), goes to (1,0) and
 *          (1/2, sqrt(3)/2) and continues counterclockwise.
 * https://oeis.org/A036496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = ceil(sqrt(12*n - 3)) for n ≥ 1, a(0) = 0
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    // compute 12*n - 3 as seq_t to avoid overflow
    seq_t m = (seq_t)12 * (seq_t)n - (seq_t)3;

    seq_t k = floor_sqrt(m);            // floor(sqrt(m))
    seq_t sq = k * k;                   // k^2

    if (sq == m)
        return k;                       // perfect square
    else
        return k + (seq_t)1;            // ceil(sqrt(m))
}
