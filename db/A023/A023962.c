/*
 * A023962  First digit after decimal point of cube root of n.
 * https://oeis.org/A023962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(10 * (∛n - floor(∛n))) = floor_cube_root(1000 * n)
 *            – 10 * floor_cube_root(n)
 * Range:     u8 none, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* compute integer cube root of n */
    seq_t r = floor_cube_root((seq_t)n);

    /* compute integer cube root of 1000*n */
    seq_t r1000 = floor_cube_root((seq_t)1000 * (seq_t)n);

    /* first decimal digit = r1000 - 10*r */
    return r1000 - (seq_t)10 * r;
}
