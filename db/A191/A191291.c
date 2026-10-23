/*
 * A191291  The number of bases >=2 in which n is 3-digit number.
 * https://oeis.org/A191291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = floor(sqrt(n)) - floor(n^(1/3)) = A000196(n) - A048766(n)
 * Range:     u8 n <= 63, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   floor_cube_root, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t for the helper functions */
    seq_t ns = (seq_t)n;
    seq_t sqrt_n = floor_sqrt(ns);
    seq_t cbrt_n = floor_cube_root(ns);
    return sqrt_n - cbrt_n;
}
