/*
 * A160869  a(n) = sigma(6^(n-1)).
 * https://oeis.org/A160869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(6^(n-1)) = ((3^n - 1)/2) * (2^n - 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (This is computed multiplicatively to avoid overflow.)   */
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* compute 2^n and 3^n */
    seq_t two_pow   = pow_uint(2, n);
    seq_t three_pow = pow_uint(3, n);

    /* (3^n - 1)/2  (division by 2 is exact) */
    seq_t part = (three_pow - 1) >> 1;

    /* result = part * (2^n - 1)  */
    seq_t result = part * (two_pow - 1);

    return result;
}
