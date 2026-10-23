/*
 * A121105  Trajectory of 679 under "x -> product of digits of x" map.
 * https://oeis.org/A121105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(1)=679; for n>1, a(n)=digit_product(a(n-1))
 * Range:     u8 none, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)679;
    if (n == 1) return x;
    for (idx_t i = 1; i < n; ++i)
    {
        x = digit_product(x);
        if (x == (seq_t)6) /* fixed point reached, all further terms are 6 */
            break;
    }
    return x;
}
