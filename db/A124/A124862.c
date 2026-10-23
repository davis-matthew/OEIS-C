/*
 * A124862  a(n) = J(2*n+1) * C(2*n,n), where J(n) = A001045(n).
 * https://oeis.org/A124862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = J(2*n+1) * C(2*n,n), where J(k)=(2^k+(-1)^k)/3 ; thus
 *            J(2*n+1) = (2^(2*n+1)+1)/3 It is equivalent to a(n) = A007583(n)
 *            * A000984(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Compute a(n) for n >= 0. */
seq_t a(idx_t n)
{
    /* Compute J(2*n+1) = (2^(2*n+1)+1)/3 */
    idx_t exp = 2 * n + 1;                 /* exponent of 2 */
    seq_t pow_two = pow2(exp);             /* 2^(2*n+1) */
    seq_t Jodd = (pow_two + 1) / (seq_t)3; /* exact division */

    /* Compute binomial coefficient C(2*n, n) multiplicatively */
    seq_t bin = 1;
    idx_t i;
    for (i = 1; i <= n; ++i) {
        bin = (bin * (n + i)) / i;         /* exact division each step */
    }

    /* The final product */
    return Jodd * bin;
}
