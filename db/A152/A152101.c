/*
 * A152101  a(n)=16^n - 3*2^(2*n - 1) - 1.
 * https://oeis.org/A152101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16^n - 3*2^(2*n - 1) - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 16^n = 2^(4n) and 2^(2*n - 1) are easily obtained via pow2. */
    idx_t exp16 = 4 * n;          /* exponent for 16^n */
    idx_t exp2  = 2 * n - 1;      /* exponent for 2^(2*n - 1) */

    seq_t p16 = pow2(exp16);      /* 16^n */
    seq_t p2  = pow2(exp2);       /* 2^(2*n - 1) */

    /* Compute the value: 16^n - 3*2^(2*n - 1) - 1 */
    seq_t result = p16 - (seq_t)3 * p2 - (seq_t)1;
    return result;
}
