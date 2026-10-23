/*
 * A120581  Hankel transform of sum{k=0..n, C(2k,k)*2^k}.
 * https://oeis.org/A120581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 4^(n+C(n,2))*(-1)^n*sum{k=0..n, C(n+k,2k)(-2)^(n-k)}
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 7, i128 n <= 10,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* compute S(n) = sum_{k=0}^n binom(n+k,2k) * (-2)^(n-k) */
    seq_t S = 0;
    seq_t T = 1;                       /* binom(n,0) */
    seq_t p = pow_uint(2, (idx_t)n);   /* 2^n */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = T * p;
        if (((n - k) & 1) != 0)       /* sign from (-2)^(n-k) */
            term = -term;
        S += term;

        if (k == n)
            break;

        /* update T for k+1: T = T * (n-k)(n+k+1) / ((2k+2)(2k+1)) */
        idx_t nk1   = n - k;
        idx_t npk1  = n + k + 1;
        seq_t numerator = (seq_t)(nk1 * npk1);

        idx_t d1 = 2 * k + 2;
        idx_t d2 = 2 * k + 1;
        seq_t denom = (seq_t)(d1 * d2);

        T = (T * numerator) / denom;

        /* divide p by 2 for the next exponent */
        p /= 2;
    }

    /* compute exponent e = n + C(n,2) = n*(n+1)/2 + n - n/2? */
    seq_t n_seq = (seq_t)n;
    seq_t e = n_seq + (n_seq * (n_seq - 1)) / 2;   /* n + n*(n-1)/2 = n + C(n,2) */

    seq_t factor = pow_uint(4, (idx_t)e);          /* 4^e */

    seq_t result = factor * S;
    if (n & 1)                                     /* (-1)^n */
        result = -result;

    return result;
}
