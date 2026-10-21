/*
 * A227713  The Wiener index of the tree g[n] (n>=0) defined recursively in
 *          the following manner: denoting by P[n] the path on n vertices, we
 *          define g[0] =P[2] while g[n] (n>=1) is the tree obtained by
 *          identifying the roots of 2 copies of g[n-1] and one of the
 *          end-vertices of P[n+1]; the root of g[n] is defi...
 * https://oeis.org/A227713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 80 +128*n/3 +19*n^2/2 +5*n^3/6 * +2^n*(-64 +38*n
 *            +21*n^2/2 +3*n^3/2) * +4^n*(-15 -27*n/2 +9*n^2/2) * *
 *            Implementation: multiply entire expression by 6, eliminate
 *            fractions, * then divide final sum by 6
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    /* Components scaled by 6 */
    seq_t t1 = (seq_t)480;                 /* 6*80 */
    seq_t t2 = (seq_t)256 * nn;            /* 6*(128*n/3) = 256*n */
    seq_t t3 = (seq_t)57 * n2;             /* 6*(19*n^2/2) = 57*n^2 */
    seq_t t4 = (seq_t)5 * n3;              /* 6*(5*n^3/6) = 5*n^3 */

    seq_t pow2n = pow2(n);                 /* 2^n */
    seq_t t5 = pow2n * ((seq_t)(-384)
                       + (seq_t)(228) * nn
                       + (seq_t)(63)  * n2
                       + (seq_t)(9)   * n3);   /* 6*2^n*(-64 +38*n +21*n^2/2 +3*n^3/2) */

    seq_t pow4n = pow4(n);                 /* 4^n */
    seq_t t6 = pow4n * ((seq_t)(-90)
                       + (seq_t)(-81) * nn
                       + (seq_t)(27)  * n2);    /* 6*4^n*(-15 -27*n/2 +9*n^2/2) */

    seq_t sum = t1 + t2 + t3 + t4 + t5 + t6;

    return sum / 6;
}
