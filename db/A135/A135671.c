/*
 * A135671  a(n) = ceiling(n - n^(2/3)).
 * https://oeis.org/A135671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n - n^(2/3)) = n - floor(n^(2/3))
 * Range:     u8 n <= 7, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* n^2 – use seq_t arithmetic to avoid overflow */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* floor(n^(2/3)) = floor(cuberoot(n^2)) */
    seq_t t = floor_cube_root(sq);

    /* a(n) = n - floor(n^(2/3)) */
    return (seq_t)n - t;
}
