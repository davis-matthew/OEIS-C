/*
 * A135674  a(n) = floor(n + n^(2/3)).
 * https://oeis.org/A135674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n^(2/3))
 * Range:     u8 n <= 7, u16 n <= 261, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    seq_t n2 = (seq_t)n * (seq_t)n;
    seq_t cr = floor_cube_root(n2);
    return (seq_t)n + cr;
}
