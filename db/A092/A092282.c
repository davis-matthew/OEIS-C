/*
 * A092282  Difference between largest square number and largest cube number
 *          less than n.
 * https://oeis.org/A092282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = floor(sqrt(n))^2 - floor(cuberoot(n))^3
 * Range:     i8 n <= 63, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   floor_cube_root, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    seq_t sq   = floor_sqrt((seq_t)n);        /* floor of sqrt(n)   */
    seq_t cube = floor_cube_root((seq_t)n);   /* floor of cbrt(n)   */
    seq_t res  = sq * sq - cube * cube * cube;
    return res;
}
