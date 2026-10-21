/*
 * A069255  a(n) = (2^((2*n-1)^2)+1)/(2^(2*n-1)+1).
 * https://oeis.org/A069255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^((2*n-1)^2) + 1) / (2^(2*n-1) + 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for n >= 1.  The implementation uses
   the provided pow2 function to build the powers of two in arbitrary
   precision. */
#include "../utils/pow2.h"   /* pow2(idx_t exp) -> seq_t */

seq_t a(idx_t n)
{
    /* k = 2*n - 1 (must be odd) */
    idx_t k = (idx_t)(2 * n - 1);

    /* t = 2^k */
    seq_t t = pow2(k);

    /* Compute numerator: 2^(k^2) + 1 = (t)^k + 1 */
    seq_t numerator = pow2(k * k);   /* 2^(k^2) */
    numerator = numerator + (seq_t)1;

    /* Compute denominator: 2^k + 1 = t + 1 */
    seq_t denominator = t + (seq_t)1;

    /* Final result */
    return numerator / denominator;
}
