/*
 * A018812  Number of lines through exactly 5 points of an n X n grid of
 *          points.
 * https://oeis.org/A018812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 ( f(n,6) - 2*f(n,5) + f(n,4) ) f(n, k) = Σ ( n - k|x|
 *            ) ( n - k|y| ) over all pairs with -n < kx < n , -n < ky < n ,
 *            gcd(|x|,|y|)=1 The sum is taken only over terms with positive
 *            factors
 * Range:     u8 n <= 8, u16 n <= 36, u32 n <= 920, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd_int.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* Largest absolute step needed for k = 4 (smallest multiplier) */
    idx_t maxd = (n == 0) ? 0 : (n - 1) / 4;

    seq_t f4 = 0, f5 = 0, f6 = 0;

    for (idx_t ix = 0; ix <= maxd; ++ix) {
        for (idx_t iy = 0; iy <= maxd; ++iy) {
            if (ix == 0 && iy == 0) continue;

            /* Primitive step check */
            if (ix == 0) {
                if (iy != 1) continue;          /* gcd(0,iy)=iy */
            } else if (iy == 0) {
                if (ix != 1) continue;          /* gcd(ix,0)=ix */
            } else {
                if (gcd_int((int)ix, (int)iy) != 1) continue;
            }

            int mul = (ix == 0 || iy == 0) ? 2 : 4;

            /* k = 4 */
            if (N > (seq_t)4 * ix && N > (seq_t)4 * iy) {
                seq_t term = (N - (seq_t)4 * ix) * (N - (seq_t)4 * iy);
                f4 += term * (seq_t)mul;
            }
            /* k = 5 */
            if (N > (seq_t)5 * ix && N > (seq_t)5 * iy) {
                seq_t term = (N - (seq_t)5 * ix) * (N - (seq_t)5 * iy);
                f5 += term * (seq_t)mul;
            }
            /* k = 6 */
            if (N > (seq_t)6 * ix && N > (seq_t)6 * iy) {
                seq_t term = (N - (seq_t)6 * ix) * (N - (seq_t)6 * iy);
                f6 += term * (seq_t)mul;
            }
        }
    }

    seq_t result = (f6 - (f5 << 1) + f4) >> 1;  /* (f6 - 2*f5 + f4)/2 */
    return result;
}
