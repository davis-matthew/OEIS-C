/*
 * A235594  Coefficient of q^4 in the polynomial NT_{n,mu}(q).
 * https://oeis.org/A235594
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*C(n-5+8,8) + 5*C(n-6+8,8) + 6*C(n-8+8,8) + 1*C(n-9+8,8)
 *            where C(x,8)=0 if x<8 (coefficients come from the polynomial
 *            2+5x+6x³+x⁴)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binom8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom8.h"

seq_t a(idx_t n)
{
    if (n < 5) return 0;

    idx_t m = n - 5;                 /* m = n-5 >= 0 */
    static const idx_t shifts[4] = {0, 1, 3, 4};
    static const seq_t coeffs[4] = {2, 5, 6, 1};
    seq_t result = 0;

    for (idx_t i = 0; i < 4; ++i) {
        if (m >= shifts[i]) {
            idx_t t = m - shifts[i] + 8;   /* ensure t >= 8 for binom8 */
            result += coeffs[i] * binom8(t);
        }
    }
    return result;
}
