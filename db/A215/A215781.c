/*
 * A215781  a(n) = ceiling(n*(sqrt(3)-1)).
 * https://oeis.org/A215781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*(sqrt(3)-1))
 * Range:     u8 n <= 4, u16 n <= 57, u32 n <= 499, u64 n <= 499,
 *            u128 n <= 499, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using the identity ceil(n*(sqrt(3)-1)) = floor(n*sqrt(3)) - n + 1 for n>0 */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute floor(n*sqrt(3)) as floor(sqrt(3*n*n)) */
    seq_t sq = (seq_t)3 * (seq_t)n * (seq_t)n;
    seq_t floor_root = floor_sqrt(sq);

    return floor_root - (seq_t)n + 1;
}
