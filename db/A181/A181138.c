/*
 * A181138  Least positive integer k such that n^2 + k is a cube.
 * https://oeis.org/A181138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((⌊∛(n^2)⌋+1)^3 - n^2)
 * Range:     u8 n <= 7, u16 n <= 164, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least positive integer k such that n^2 + k is a cube */

#include <stdint.h>
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n) {
    seq_t sq = (seq_t)n * (seq_t)n;       /* n^2 */
    seq_t b  = floor_cube_root(sq);       /* floor(cuberoot(n^2)) */
    seq_t b1 = b + 1;                     /* next integer after the cube root */
    seq_t cube = b1 * b1 * b1;            /* (b+1)^3 */
    return cube - sq;                     /* smallest k > 0 with n^2 + k = cube */
}
