/*
 * A224214  Number of nonnegative solutions to x^3 + y^3 <= n^3.
 * https://oeis.org/A224214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of nonnegative integer pairs (x, y) such that x^3
 *            + y^3 <= n^3. Computation: a(n) = Σ_{x=0}^{n} (floor((n^3 -
 *            x^3)^(1/3)) + 1)
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 50 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
    seq_t count = 0;
    seq_t nn = (seq_t)n;
    seq_t n3 = nn * nn * nn;          /* n³ */

    for (idx_t x = 0; x <= n; ++x) {
        seq_t xx = (seq_t)x;
        seq_t x3 = xx * xx * xx;      /* x³ */
        seq_t rem = n3 - x3;          /* n³ – x³  */
        seq_t y_max = floor_cube_root(rem); /* floor(cuberoot(rem)) */
        count += y_max + 1;           /* y from 0 to y_max inclusive */
    }

    return count;
}
