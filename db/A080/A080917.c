/*
 * A080917  Number of integer solutions to the equation 2*x^2 + y^2 + 8*z^2 =
 *          n.
 * https://oeis.org/A080917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (x,y,z) to 2*x^2 + y^2 +
 *            8*z^2 = n (x,y,z ∈ ℤ)
 * Range:     u8 n <= 1768, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u64_floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h> /* for bool, not strictly required but harmless */
#include "../utils/u64_floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t total = 0;

    /* max |z| with 8*z^2 <= n */
    idx_t zm = u64_floor_sqrt(n / 8);
    for (idx_t z = 0; z <= zm; ++z) {
        seq_t z_mult = (z == 0) ? 1 : 2;
        idx_t zsq   = z * z;
        idx_t rem1  = n - 8 * zsq;          /* remainder after z term */

        /* max |y| with y^2 <= rem1 */
        idx_t ym = u64_floor_sqrt(rem1);
        for (idx_t y = 0; y <= ym; ++y) {
            seq_t y_mult = (y == 0) ? 1 : 2;
            idx_t ysq   = y * y;
            idx_t rem2  = rem1 - ysq;       /* remainder after y term */

            if (rem2 & 1ULL) continue;     /* must be even for 2*x^2 */

            idx_t x2 = rem2 >> 1;           /* x^2 = x2 */

            seq_t x_mult;
            if (x2 == 0) {
                x_mult = 1;                 /* x = 0 has one sign */
            } else {
                idx_t xr = u64_floor_sqrt(x2);
                x_mult = (xr * xr == x2) ? 2 : 0;  /* 0 if not a square */
            }

            if (x_mult) {
                total += z_mult * y_mult * x_mult;
            }
        }
    }

    return total;
}
