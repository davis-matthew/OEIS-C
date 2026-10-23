/*
 * A198274  a(n) = 13*2^n-1.
 * https://oeis.org/A198274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*2^n-1
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   pow2_binexp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2_binexp.h"
/* Uses binary exponentiation to compute 2^n efficiently. */

seq_t a(idx_t n)
{
    seq_t v = pow2_binexp(n);   /* v = 2^n */
    v *= (seq_t)13;             /* v = 13 * 2^n */
    v -= (seq_t)1;              /* v = 13 * 2^n - 1 */
    return v;
}
