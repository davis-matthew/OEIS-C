/*
 * A124753  a(3n+k) = (k+1)*binomial(4n+k, n)/(3n+k+1), where k is n reduced
 *          mod 3.
 * https://oeis.org/A124753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n mod 3)+1)*C(4*floor(n/3)+n mod 3,
 *            floor(n/3))/(3*floor(n/3)+1+n mod 3)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 29, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t m = n / 3;            /* floor(n/3)          */
    idx_t k = n % 3;            /* n mod 3              */
    idx_t N = 4 * m + k;        /* 4*floor(n/3)+mod(n,3) */

    seq_t bin = binomial(N, m);             /* C(N, m)           */
    seq_t numerator   = bin * (seq_t)(k + 1);/* (k+1)*C(N,m)      */
    seq_t denominator = (seq_t)(3 * m + 1 + k);/* 3*m+1+k           */

    return numerator / denominator; /* exact division   */
}
