/*
 * A058877  Number of labeled acyclic digraphs with n nodes containing exactly
 *          n-1 points of in-degree zero.
 * https://oeis.org/A058877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*2^(n-1) – n, offset = 1 (a(1) = 0)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For safety handle the offset boundary; the sequence is defined for n >= 1. */
    if (n == 0) return 0;

    /* Compute 2^(n-1) by repeated multiplication by 2. */
    seq_t pow2 = 1;
    for (idx_t i = 1; i < n; ++i)
        pow2 = pow2 * 2;

    /* Convert n to the proper type and apply the formula. */
    seq_t ns = (seq_t)n;
    return ns * pow2 - ns;
}
