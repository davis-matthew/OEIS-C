/*
 * A017913  Powers of sqrt(3) rounded down.
 * https://oeis.org/A017913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (sqrt(3))^n )
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 80, u128 n <= 64,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 959 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation via integer arithmetic: */
/*   iterate x + y*sqrt(3) = (sqrt(3))^n  */
/*   then a(n) = x + floor( y*sqrt(3) )  */
/*   floor( y*sqrt(3) ) = floor( sqrt( 3*y^2 ) ) */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)1;   /* x_0 = 1, y_0 = 0 */
    seq_t y = (seq_t)0;

    for (idx_t i = 0; i < n; ++i) {
        seq_t new_x = y * (seq_t)3;
        seq_t new_y = x;
        x = new_x;
        y = new_y;
    }

    seq_t y2 = y * y;        /* y^2   */
    seq_t temp = y2 * (seq_t)3;  /* 3*y^2 */
    seq_t sqrt_floor_val = floor_sqrt(temp); /* floor(sqrt(3*y^2)) */
    return x + sqrt_floor_val;
}
