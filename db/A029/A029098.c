/*
 * A029098  Expansion of 1/((1-x)*(1-x^5)*(1-x^8)*(1-x^11)).
 * https://oeis.org/A029098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((110*n^3+45*n^2+160*n+96)/96) * -
 *            floor((3*n^3+3*n+1)/5) * - floor((6*n^3+5*n^2+10*n+5)/11)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 416, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* convert index to the seq_t type early */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    /* compute each floor term */
    seq_t term1 = ((seq_t)110 * n3 + (seq_t)45 * n2 + (seq_t)160 * nn + (seq_t)96) / (seq_t)96;
    seq_t term2 = ((seq_t)3 * n3 + (seq_t)3 * nn + (seq_t)1) / (seq_t)5;
    seq_t term3 = ((seq_t)6 * n3 + (seq_t)5 * n2 + (seq_t)10 * nn + (seq_t)5) / (seq_t)11;

    return term1 - term2 - term3;
}
