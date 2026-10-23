/*
 * A178631  a(n) = 27*((10^n - 1)/9)^2.
 * https://oeis.org/A178631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*((10^n - 1)/9)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    // Compute t = ((10^n - 1)/9)^2 * 27
    seq_t t = pow10(n);          /* 10^n   */
    t -= 1;                      /* 10^n-1 */
    t /= 9;                      /* (10^n-1)/9 */
    t *= t;                      /* square */
    t *= 27;                     /* multiply by 27 */
    return t;
}
