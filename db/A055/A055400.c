/*
 * A055400  Cube excess: difference between n and largest cube <= n.
 * https://oeis.org/A055400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n - floor(cuberoot(n))^3
 * Range:     u8 n <= 984, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   cube_root_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/cube_root_floor.h"

seq_t a(idx_t n)
{
    /* get integer cube root floor of n */
    idx_t k = cube_root_floor(n);

    /* compute k^3 explicitly as seq_t to avoid overflow */
    seq_t cube = (seq_t)k;
    cube = cube * (seq_t)k;
    cube = cube * (seq_t)k;

    /* a(n) = n - k^3 */
    return (seq_t)n - cube;
}
