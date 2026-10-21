/*
 * A066373  a(n) = (3*n-2)*2^(n-3).
 * https://oeis.org/A066373
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n-2)*2^(n-3) (offset 2: a(2) is the first term)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    // Undefined for n < 2; return 0 safely
    if (n < 2)
        return 0;

    seq_t coeff = (seq_t)(3*n - 2);

    if (n == 2)              /* 2^(2-3) = 2^-1 -> divide by 2 */
        return coeff / 2;

    /* For n >= 3, compute 2^(n-3) normally */
    seq_t pow = pow2(n - 3);
    return coeff * pow;
}
