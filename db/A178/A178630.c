/*
 * A178630  a(n) = 18*((10^n - 1)/9)^2.
 * https://oeis.org/A178630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*((10^n - 1)/9)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^n */
    seq_t v = pow10(n);
    v -= 1;          /* (10^n - 1) */
    v /= 9;          /* integer division: (10^n - 1) / 9 */
    v = v * v;       /* square */
    v *= 18;         /* multiply by 18 */
    return v;
}
