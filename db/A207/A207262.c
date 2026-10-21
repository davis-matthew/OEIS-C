/*
 * A207262  a(n) = 2^(4n - 2) + 1.
 * https://oeis.org/A207262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(4n - 2) + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; behaviour for n = 0 is undefined. */
    if (n == 0) {
        return (seq_t)0;
    }
    /* Compute the exponent 4*n - 2.  4*n fits in idx_t for the values of n we
       support (up to about 10,000).  The subtraction is still in idx_t.
    */
    idx_t exp = 4 * n - 2;
    seq_t val = pow2(exp);   /* 2^(4n-2) */
    val += (seq_t)1;         /* +1 */
    return val;
}
