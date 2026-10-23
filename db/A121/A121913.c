/*
 * A121913  a(n) = 2^(n*(2*n+3)) = 2^A014106(n).
 * https://oeis.org/A121913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n*(2*n+3))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute the exponent n*(2*n+3) as an idx_t integer. */
    idx_t exp = n * ((n * 2) + 3);
    /* Return 2 raised to that exponent. */
    return pow2(exp);
}
