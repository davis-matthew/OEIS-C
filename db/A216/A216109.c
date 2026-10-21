/*
 * A216109  The hyper-Wiener index of the ortho-polyphenyl chain with n
 *          hexagons (see the Dou et al. and the Deng references).
 * https://oeis.org/A216109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(28+123*n+127*n^2+36*n^3+4*n^4)/2
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cast the index to seq_t to avoid undefined behaviour on very large n */
    seq_t nn = (seq_t)n;

    /* Compute powers of n */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;

    /* Build the polynomial */
    seq_t temp = (seq_t)28
               + (seq_t)123 * nn
               + (seq_t)127 * n2
               + (seq_t)36  * n3
               + (seq_t)4   * n4;

    /* Multiply by 3 and divide by 2 (exact division) */
    seq_t num = (seq_t)3 * temp;
    return num / (seq_t)2;
}
