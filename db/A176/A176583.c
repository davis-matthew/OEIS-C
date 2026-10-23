/*
 * A176583  n^2+smallest cube >= n^2.
 * https://oeis.org/A176583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)^2 + smallest cube ≥ (n-1)^2
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* Helper for floor cube root of a seq_t value */
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t and shift for OEIS offset */
    seq_t m = (seq_t)n - 1;          /* m = n-1 */
    seq_t m2 = m * m;                /* m^2 */
    seq_t c = floor_cube_root(m2);   /* floor(cuberoot(m^2)) */
    seq_t c3 = c * c * c;            /* c^3 */

    /* If c^3 < m^2, increase to next cube */
    if (c3 < m2) {
        c++;
        c3 = c * c * c;
    }

    return m2 + c3;                  /* a(n) */
}
