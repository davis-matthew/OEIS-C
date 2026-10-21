/*
 * A089974  (n+1)-st term of the n-th binomial transform of this sequence
 *          equals 1 for all n>=0.
 * https://oeis.org/A089974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * C(n, k) * n * k^(n-k-1)
 *            Implemented directly. a(0)=1, a(1)=0
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 15, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the base cases */
    if (n == 0) return 1;
    if (n == 1) return 0;

    seq_t sum = 0;
    /* binom holds C(n, k) for the current k.  Start with k = 1. */
    seq_t binom = (seq_t)n;  /* C(n,1) = n */

    for (idx_t k = 1; k < n; ++k) {
        /* Determine the sign (-1)^(n-k) */
        seq_t sign = ((n - k) & 1) ? (seq_t)-1 : (seq_t)1;

        /* k^(n-k-1)  (k ≥ 1, exponent ≥ 0) */
        seq_t pow_term = big_pow((seq_t)k, (seq_t)(n - k - 1));

        /* term = sign * C(n,k) * n * k^(n-k-1) */
        seq_t term = binom;
        term *= (seq_t)n;
        term *= pow_term;
        term *= sign;

        sum += term;

        /* Update binom for the next k: C(n, k+1) = C(n, k) * (n-k)/(k+1) */
        binom = binom * (seq_t)(n - k);
        binom = binom / (seq_t)(k + 1);
    }

    /* Add the k = n term, which is always +1 */
    return sum + 1;
}
