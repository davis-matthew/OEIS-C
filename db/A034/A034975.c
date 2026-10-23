/*
 * A034975  One seventh of octo-factorial numbers.
 * https://oeis.org/A034975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n = 1, otherwise a(n) = ∏_{j=2..n} (8*j-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The product form avoids the division by 7 and is valid for all    */
/* n ≥ 1.  For n = 0 the function returns 0 (undefined for this OEIS) */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but harmless      */
    if (n == 1) return 1;              /* a(1) = 1                      */

    seq_t prod = 1;
    for (idx_t j = 2; j <= n; ++j)
        prod *= (seq_t)(8 * j - 1);     /* multiply by (8*j-1)           */
    return prod;
}
