/*
 * A171472  a(n) = 6*a(n-1) - 8*a(n-2) for n > 1; a(0) = 7, a(1) = 30.
 * https://oeis.org/A171472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*4^n – 2^n Where 4^n and 2^n are computed with pow4(n)
 *            and pow2(n)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Compute 8·4^n.  The cast ensures 8 is converted to seq_t. */
    seq_t res = (seq_t)8 * pow4(n);

    /* Subtract 2^n. */
    res -= pow2(n);

    return res;
}
