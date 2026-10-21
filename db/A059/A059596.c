/*
 * A059596  Eighth column (m=7) of convolution triangle A059594(n,m).
 * https://oeis.org/A059596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by a(2k ) = binom(k+11,11) *
 *            (16k^4+384k^3+2936k^2+7584k+4095) / 4095 a(2k+1 ) =
 *            binom(k+12,12) * 8*(2k+5)*(2k+13)*(2k+21) / 1365
 * Range:     u8 none, u16 n <= 1, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=1, a(1)=8, a(2)=44, …  (OEIS A059596) */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                /* n = 2k  */
        idx_t k      = n >> 1;
        seq_t bin    = binom(k + 11, 11);   /* C(k+11,11) */

        seq_t kseq   = (seq_t)k;
        seq_t k2     = kseq * kseq;         /* k^2   */
        seq_t k3     = k2   * kseq;         /* k^3   */
        seq_t k4     = k3   * kseq;         /* k^4   */

        /* polynomial part: 16k^4+384k^3+2936k^2+7584k+4095 */
        seq_t poly   = 16 * k4 + 384 * k3 + 2936 * k2 + 7584 * kseq + 4095;

        seq_t num    = bin * poly;
        return num / 4095;                  /* 15*7*13*3 */
    } else {                                 /* n = 2k+1 */
        idx_t k      = (n - 1) >> 1;
        seq_t bin    = binom(k + 12, 12);   /* C(k+12,12) */

        seq_t kseq   = (seq_t)k;
        seq_t two_k  = 2 * kseq;
        seq_t t1     = two_k + 5;
        seq_t t2     = two_k + 13;
        seq_t t3     = two_k + 21;

        /* polynomial part: 8*(2k+5)*(2k+13)*(2k+21) */
        seq_t poly   = 8 * t1 * t2 * t3;

        seq_t num    = bin * poly;
        return num / 1365;                  /* 15*7*13  */
    }
}
