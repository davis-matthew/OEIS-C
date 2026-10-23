/*
 * A121536  Smallest m such that m^3 >= n^2.
 * https://oeis.org/A121536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n-1)^(2/3)) = smallest integer m such that m^3 >=
 *            (n-1)^2
 * Range:     u8 n <= 8, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* Handle n=0 defensively, though offset starts at 1 */
    if (n == 0) return 0;

    /* Compute k = n-1 so that a(n) = ceil((k^2)^(1/3)) */
    seq_t k = (seq_t)n - 1;
    seq_t v = k * k;                 /* k^2 */
    seq_t m = floor_cube_root(v);    /* floor(cuberoot(v)) */
    seq_t m3 = m * m * m;            /* m^3 */

    /* If m^3 < v, increment m to get the ceiling */
    if (m3 < v) m = m + 1;

    return m;
}
