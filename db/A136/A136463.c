/*
 * A136463  Diagonal of square array A136462: a(n) = C((n+1)*2^(n-1), n) for
 *          n>=0.
 * https://oeis.org/A136463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C((n+1)*2^(n-1), n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                    /* a(0) = 1 by definition */

    /* Compute N = (n+1) * 2^(n-1) */
    seq_t pow2_val = pow2(n - 1);              /* 2^(n-1) */
    seq_t N = (seq_t)(n + 1) * pow2_val;        /* (n+1)*2^(n-1) */

    /* Use multiplicative formula: C(N,n) = Π_{i=1..n} (N - n + i)/i */
    seq_t N_minus_n = N - n;                   /* N - n, reused each loop */
    seq_t result = 1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t numerator = N_minus_n + i;
        result = result * numerator;
        result = result / (seq_t)i;           /* division is exact */
    }
    return result;
}
