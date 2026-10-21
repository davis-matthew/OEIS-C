/*
 * A067474  Smallest n-digit square starting with 4.
 * https://oeis.org/A067474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ceil(sqrt(4 * 10^(n-1)))^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n >= 1 according to the OEIS definition. */
    idx_t exp  = n - 1;                           /* exponent for 10^(n-1)   */
    seq_t target = 4 * pow10(exp);                /* 4 * 10^(n-1)            */
    seq_t r = floor_sqrt(target);                 /* floor(sqrt(target))     */
    if (r * r < target)                          /* round up if necessary   */
        r = r + 1;
    return r * r;                                 /* square the ceiling      */
}
