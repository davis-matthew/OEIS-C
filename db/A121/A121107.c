/*
 * A121107  Trajectory of 68889 under "x -> product of digits of x" map.
 * https://oeis.org/A121107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(1) = 68889; for n>1, a(n) = digit_product(a(n-1))
 * Range:     u8 none, u16 none, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function digit_product is provided by the utilities.
   It returns the product of the decimal digits of its argument. */
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for offset, return 0 */

    seq_t val = 68889;           /* a(1) */

    for (idx_t i = 1; i < n; ++i) /* apply n-1 transitions */
    {
        if (val == 0) break;    /* once zero is reached, it stays zero */
        val = digit_product(val);
    }

    return val;
}
