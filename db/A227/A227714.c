/*
 * A227714  The hyper-Wiener index of the tree g[n] (n>=0) defined recursively
 *          in the following manner: denoting by P[n] the path on n vertices,
 *          we define g[0] =P[2] while g[n] (n>=1) is the tree obtained by
 *          identifying the roots of 2 copies of g[n-1] and one of the
 *          end-vertices of P[n+1]; the root of g[n] i...
 * https://oeis.org/A227714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -2026 -14671*n/12 -8077*n^2/24 -605*n^3/12 -83*n^4/24 * +
 *            2^n*(2961/2 -9783*n/20 -1049*n^2/8 -147*n^3/8 +9*n^4/8
 *            +21*n^5/40) * + 4^n*(1093/2-279*n/4 +51*n^2/2 -27*n^3/2
 *            +9*n^4/4)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t nst = (seq_t)n;
    seq_t n2   = nst * nst;
    seq_t n3   = n2 * nst;
    seq_t n4   = n3 * nst;
    seq_t n5   = n4 * nst;

    /* coefficients for the 2^n part after scaling by 120   */
    seq_t Bcoeff = (seq_t)177660
                  - (seq_t)58698 * nst
                  - (seq_t)15735 * n2
                  - (seq_t)2205  * n3
                  + (seq_t)135   * n4
                  + (seq_t)63    * n5;

    /* coefficients for the 4^n part after scaling by 120   */
    seq_t Ccoeff = (seq_t)65580
                  - (seq_t)8370 * nst
                  + (seq_t)3060 * n2
                  - (seq_t)1620 * n3
                  + (seq_t)270  * n4;

    seq_t pow2 = seq_pow((seq_t)2, n);
    seq_t pow4 = seq_pow((seq_t)4, n);

    seq_t raw = pow2 * Bcoeff + pow4 * Ccoeff;

    /* subtract the scaled polynomial part   */
    raw -= (seq_t)243120;
    raw -= (seq_t)146710 * nst;
    raw -= (seq_t)40385  * n2;
    raw -= (seq_t)6050   * n3;
    raw -= (seq_t)415    * n4;

    return raw / (seq_t)120;
}
