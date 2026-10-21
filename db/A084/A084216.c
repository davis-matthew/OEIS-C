/*
 * A084216  a(n) = smallest integer d such that a quadratic representation
 *          2n+1 = x^2 + d*y^2 exists (x,y positive integers).
 * https://oeis.org/A084216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer d such that 2*n+1 = x^2 + d*y^2
 *            for some positive integers x and y
 * Range:     u8 n <= 16, u16 n <= 3737, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_isqrt.h"

seq_t a(idx_t n)
{
    /* 2*n + 1 can be computed safely in seq_t; the cast makes the
       multiplication happen in the larger seq_t type. */
    seq_t target = (seq_t)n * 2 + 1;

    for (seq_t d = 1;; ++d) {            /* try d = 1,2,3,… */
        seq_t d_ymax = _isqrt(target / d);   /* floor(sqrt(target/d)) */
        for (seq_t y = 1; y <= d_ymax; ++y) {
            seq_t rem = target - d * y * y;  /* remainder = x^2 */
            if (rem == 0)                   /* x would be 0 → illegal */
                continue;
            seq_t xr = _isqrt(rem);
            if (xr * xr == rem)              /* perfect square found */
                return d;
        }
    }
}
