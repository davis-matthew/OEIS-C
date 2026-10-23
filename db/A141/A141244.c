/*
 * A141244  Numerators in the expansion of (1-sqrt(1-x^2))/(1-x).
 * https://oeis.org/A141244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = (4^m - C(2m, m)) / 2^v2(4^m - C(2m, m)) where m = n/2
 *            (integer division). a(0)=0,a(1)=0. The numerator is obtained by
 *            removing all factors of 2 from the difference of 4^m and the
 *            central binomial coefficient
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = floor(n/2) */
    idx_t m = n >> 1;
    if (m == 0)
        return (seq_t)0;

    /* compute 4^m as 1 * 4^m (loop to avoid large shifts) */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < m; ++i)
        pow2 *= (seq_t)4;      /* 4^m  */

    /* compute central binomial coefficient C(2m, m) iteratively */
    seq_t binom = (seq_t)1;
    for (idx_t k = 1; k <= m; ++k) {
        /* binom = binom * (m + k) / k;  division is exact */
        binom = binom * (seq_t)(m + k);
        binom /= (seq_t)k;
    }

    /* difference */
    seq_t T = pow2 - binom;

    /* remove all factors of 2 (i.e., divide by 2^v2) */
    while ((T & (seq_t)1) == (seq_t)0) {
        T >>= 1;
    }

    return T;
}
