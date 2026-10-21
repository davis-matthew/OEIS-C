/*
 * A097449  If n is a cube, replace it with the cube root of n.
 * https://oeis.org/A097449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n, unless n is a perfect cube, in which case a(n) = ∛n
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   int_cuberoot
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_cuberoot.h"

seq_t a(idx_t n)
{
    idx_t r = int_cuberoot(n);
    if ((__int128)r * r * r == (__int128)n)
        return (seq_t)r;   /* replace perfect cube by its integer cube root */
    return (seq_t)n;        /* otherwise keep n */
}
