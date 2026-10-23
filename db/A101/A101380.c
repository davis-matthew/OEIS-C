/*
 * A101380  a(n) = n^2*(n+1)*(4*n^3 - 2*n^2 + n + 3)/12.
 * https://oeis.org/A101380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n+1)*(4*n^3 - 2*n^2 + n + 3)/12
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 48, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_seq_local_calc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq_local_calc.h"

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;               /* n in seq_t */

    /* polynomial part: 4*n^3 - 2*n^2 + n + 3 */
    seq_t poly = 4 * t * t * t;
    poly = poly - 2 * t * t;          /* 4*n^3 - 2*n^2 */
    poly = poly + t;                  /* + n */
    poly = poly + 3;                  /* + 3 */

    /* numerator factors: n, n, (n+1), poly */
    seq_t nums[4] = { t, t, t + 1, poly };
    seq_t den = 12;                   /* denominator */

    /* cancel common factors with the denominator */
    for (int i = 0; i < 4 && den > 1; ++i) {
        seq_t g = gcd_seq_local_calc(nums[i], den);
        if (g > 1) {
            nums[i] /= g;
            den    /= g;
        }
    }

    /* compute the product – after cancellation `den` should be 1 */
    seq_t res = nums[0];
    res *= nums[1];
    res *= nums[2];
    res *= nums[3];

    return res;
}
