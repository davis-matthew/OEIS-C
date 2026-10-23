/*
 * A136466  Row 2 of square array A136462: a(n) = C(3*2^(n-1), n) for n>=0.
 * https://oeis.org/A136466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(3*2^(n-1), n) for n >= 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    /* Upper argument of the binomial: 3 * 2^(n-1) */
    seq_t N = (seq_t)3 * pow2(n - 1);    /* pow2(exp) returns 2^exp as seq_t */

    seq_t res = (seq_t)1;
    seq_t k   = (seq_t)n;                /* value of n as seq_t */

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute (N - n + i) and do the exact division by i */
        seq_t term = N - k + (seq_t)i;
        res = (res * term) / (seq_t)i;  /* division is exact */
    }
    return res;
}
