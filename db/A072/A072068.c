/*
 * A072068  Number of integer solutions to the equation 2x^2+y^2+8z^2=m for an
 *          odd number m=2n-1.
 * https://oeis.org/A072068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 2833, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculates a(n) = number of integer solutions of 2x² + y² + 8z² = 2n‑1
   (with n ≥ 1).  The implementation iterates over all feasible z,
   then over all x, and tests whether the remaining part is a square
   for y.  It uses the integer–square‑root helper from
   utils/sqrt_floor.h.  The complexity is O(√m) ~ O(√n).  */
#include <stddef.h>
#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    /* m = 2n-1 is odd */
    idx_t m = ((idx_t)2) * n - 1;

    seq_t ans = 0;

    /* maximum |z| such that 8z² ≤ m */
    idx_t z_max = (idx_t)sqrt_floor((seq_t)m / 8);
    for (idx_t z = 0; z <= z_max; ++z) {
        seq_t zterm = (seq_t)8 * z * z;
        seq_t rem   = (seq_t)m - zterm;          /* remaining to be 2x² + y² */

        /* sign choices for z (0 has 1, else 2) */
        seq_t zsign = (z == 0) ? (seq_t)1 : (seq_t)2;

        /* maximal |x|: 2x² ≤ rem → x² ≤ rem/2 */
        idx_t x_max = (idx_t)sqrt_floor(rem / 2);
        for (idx_t x = 0; x <= x_max; ++x) {
            seq_t x2 = (seq_t)x * (seq_t)x;
            seq_t t  = rem - (seq_t)2 * x2;       /* t = y², must be ≥ 0 */

            if (t < 0) continue;

            seq_t y = sqrt_floor(t);
            if (y * y != t) continue;            /* not a perfect square */

            /* sign choices for x and y (0 has 1, else 2 each) */
            seq_t signs = ((x == 0) ? (seq_t)1 : (seq_t)2) *
                          ((y == 0) ? (seq_t)1 : (seq_t)2);

            ans += signs * zsign;
        }
    }

    return ans;
}
