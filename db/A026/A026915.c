/*
 * A026915  a(n) = T(n,0) + T(n,1) + ... + T(n,n), T given by A026907.
 * https://oeis.org/A026915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 49*2^n - 24*n - 48
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the helper pow2(idx_t exp) from ../utils/pow2.h. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 49*2^n */
    seq_t val = pow2(n);
    val *= (seq_t)49;

    /* Subtract 24*n */
    val -= (seq_t)24 * (seq_t)n;

    /* Subtract 48 */
    val -= (seq_t)48;

    return val;
}
