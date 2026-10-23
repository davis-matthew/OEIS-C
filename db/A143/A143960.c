/*
 * A143960  a(n) = the n-th positive integer with exactly n zeros and n ones
 *          in its binary representation.
 * https://oeis.org/A143960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 2^(2*n-1) + 2^n - 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The formula holds for all n >= 1.  For n < 1 the result is
       undefined but the function is never called with such a value. */
    seq_t term1 = pow2(2*n - 1);
    seq_t term2 = pow2(n);
    return term1 + term2 - 2;
}
