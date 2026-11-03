/*
 * A176583  n^2+smallest cube >= n^2.
 * https://oeis.org/A176583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)^2 + smallest cube ≥ (n-1)^2
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* Helper for floor cube root of a seq_t value */
#include "../utils/ipow_i32.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
