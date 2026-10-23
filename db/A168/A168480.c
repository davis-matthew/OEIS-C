/*
 * A168480  G.f.: Sum_{n>=0} 2^(n^2)*(1 + 2^n*x)^n*x^n.
 * https://oeis.org/A168480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n-k, k) * 2^(n*(n-k))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t half   = n / 2;
    /* 2^(n*n) – the starting value of the power of two in the sum. */
    idx_t exp = n * n;
    seq_t pow  = pow2(exp);   /* 2^(n*n) */
    /* 2^n – the common factor to divide at each step. */
    seq_t pow_n = pow2(n);
    /* binomial(n - k, k), initialized for k = 0. */
    seq_t bin = 1;

    for (idx_t k = 0; k <= half; ++k)
    {
        /* Accumulate the current term. */
        result += bin * pow;

        if (k < half)
        {
            /* Compute the next binomial coefficient using the relation:
               C(n-(k+1), (k+1)) = C(n-k, k) * (n-2k)(n-2k-1) / ((n-k)(k+1)) */
            seq_t num = (seq_t)(n - 2 * k) * (seq_t)(n - 2 * k - 1);
            seq_t den = (seq_t)(n - k) * (seq_t)(k + 1);
            bin = (bin * num) / den;

            /* Reduce the power of two: 2^(n*(n-(k+1))) = 2^(n*(n-k)) / 2^n. */
            pow = pow / pow_n;
        }
    }

    return result;
}
