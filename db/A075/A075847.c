/*
 * A075847  Difference between n^2 and the largest cube <= n^2.
 * https://oeis.org/A075847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 − ⌊∛(n^2)⌋^3 (difference between n^2 and the largest
 *            cube ≤ n^2)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   floor_cuberoot
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_cuberoot.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t sq = N * N;
    seq_t c  = floor_cuberoot(sq);
    return sq - (c * c * c);
}
