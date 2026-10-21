/*
 * A059598  Tenth column (m=9) of convolution triangle A059594(n,m).
 * https://oeis.org/A059598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A059598: 10th column of convolution triangle A059594
   Formula (OEIS):
   a(2k)   = binomial(k+14,14)*(2*k+15)*(8*k^4+240*k^3+2185*k^2+5775*k+2907)/(19*9*17*15)
   a(2k+1) = binomial(k+15,15)*2*(8*k^4+256*k^3+2767*k^2+11504*k+14535)/(17*9*19)
*/

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t k = n >> 1;               /* k = floor(n/2)          */
    if ((n & 1) == 0) {             /* even index               */
        seq_t kk1 = (seq_t)k;
        seq_t kk2 = kk1 * kk1;                 /* k^2   */
        seq_t kk3 = kk2 * kk1;                 /* k^3   */
        seq_t kk4 = kk3 * kk1;                 /* k^4   */

        seq_t poly = 8*kk4;                        /* 8*k^4            */
        poly += 240*kk3;                          /* +240*k^3         */
        poly += 2185*kk2;                         /* +2185*k^2        */
        poly += 5775*kk1;                         /* +5775*k          */
        poly += 2907;                             /* +2907            */

        seq_t bin = binomial(k + 14, 14);

        seq_t num = bin * ((seq_t)(2*k + 15) * poly);   /* numerator       */
        return num / (43605u);  /* denominator 19*9*17*15 = 43605 */
    } else {                               /* odd index               */
        seq_t kk1 = (seq_t)k;
        seq_t kk2 = kk1 * kk1;                 /* k^2   */
        seq_t kk3 = kk2 * kk1;                 /* k^3   */
        seq_t kk4 = kk3 * kk1;                 /* k^4   */

        seq_t poly = 8*kk4;                        /* 8*k^4            */
        poly += 256*kk3;                          /* +256*k^3         */
        poly += 2767*kk2;                         /* +2767*k^2        */
        poly += 11504*kk1;                        /* +11504*k         */
        poly += 14535;                            /* +14535           */

        seq_t bin = binomial(k + 15, 15);

        seq_t num = bin * 2 * poly;                 /* numerator       */
        return num / (2907u);  /* denominator 17*9*19 = 2907 */
    }
}
