/*
 * A064524  Number of noncubes <= n.
 * https://oeis.org/A064524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - floor(CubeRoot(n))
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;           /* convert index to seq_t */
    seq_t cr = floor_cube_root(sn);/* floor of the cubic root of n */
    return sn - cr;                /* number of noncubes <= n */
}
