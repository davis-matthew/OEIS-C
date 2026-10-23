/*
 * A010036  Sum of 2^n, ..., 2^(n+1) - 1.
 * https://oeis.org/A010036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=2^n}^{2^{n+1}-1} k = 3*2^{2n-1} - 2^{n-1} (n≥1),
 *            a(0)=1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* 2^{2n-1} = 2^{2*n - 1} */
    idx_t exp_two_n_minus_one = (n << 1) - 1;   /* 2*n - 1  */
    seq_t pow2_two_n_m1 = pow2(exp_two_n_minus_one);

    /* 2^{n-1} */
    seq_t pow2_n_m1 = pow2(n - 1);

    /* a(n) = 3 * 2^{2n-1} - 2^{n-1} */
    seq_t res = pow2_two_n_m1 * (seq_t)3;
    res -= pow2_n_m1;
    return res;
}
