/*
 * A000155  Nearest integer to modified Bessel function K_n(1).
 * https://oeis.org/A000155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} (-1)^k * (n-1-k)!/k! *
 *            2^(n-1-2k), a(0) = 0 (= Integral_{x=0..oo} ChebyshevU(n-1,
 *            x)/exp(x) dx, the integer part of the asymptotic series of
 *            K_n(1); the dropped terms are O(1/n))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t m = n - 1;
    idx_t K = m / 2;
    /* t_K = (m-K)!/K! * 2^(m-2K) */
    seq_t t = (m % 2 == 0) ? (seq_t)1 : ((seq_t)K + 1) * 2;
    /* R_k = t_k - R_{k+1}, nonnegative because the t_k decrease */
    seq_t R = t;
    for (idx_t k = K; k >= 1; k--) {
        /* t_{k-1} = t_k * 4 * (m-k+1) * k */
        t = t * 4 * ((seq_t)(m - k + 1)) * ((seq_t)k);
        R = t - R;
    }
    return R;
}
