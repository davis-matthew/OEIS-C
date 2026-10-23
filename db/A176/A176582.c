/*
 * A176582  n^2+Largest cube, (Largest cube <= n^2).
 * https://oeis.org/A176582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)^2 + largest cube <= (n-1)^2
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* Shift the index by one so that a(1) corresponds to n = 0. */
    idx_t m = n - 1;               /* m = n-1 */

    seq_t msq = (seq_t)m * (seq_t)m;   /* (n-1)^2 */

    seq_t c   = floor_cube_root(msq);   /* floor(cuberoot((n-1)^2)) */
    seq_t c3  = c * c * c;              /* c^3 */

    return msq + c3;                   /* (n-1)^2 + c^3 */
}
