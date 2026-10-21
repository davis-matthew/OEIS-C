/*
 * A059595  Seventh column (m=6) of convolution triangle A059594(n,m).
 * https://oeis.org/A059595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*k) = binomial(k+10,10) * (16*k^3+212*k^2+708*k+429) /
 *            (13*3*11) a(2*k+1) = binomial(k+10,10) *
 *            (16*k^3+316*k^2+1852*k+3003) / (13*3*11)
 * Range:     u8 n <= 0, u16 n <= 9, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t k = n >> 1;                     /* floor(n/2) */
    seq_t bin = binomial(k + 10, 10);     /* binomial(k+10,10) */

    seq_t kk  = (seq_t)k;
    seq_t kk2 = kk * kk;
    seq_t kk3 = kk2 * kk;

    seq_t num;
    if ((n & 1) == 0) {
        num = (seq_t)16 * kk3 + (seq_t)212 * kk2 + (seq_t)708 * kk + (seq_t)429;
    } else {
        num = (seq_t)16 * kk3 + (seq_t)316 * kk2 + (seq_t)1852 * kk + (seq_t)3003;
    }

    seq_t denom = (seq_t)429;              /* 13*3*11 */
    return bin * num / denom;
}
