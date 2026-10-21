/*
 * A073376  Sixth convolution of A001045(n+1) (generalized (1,2)-Fibonacci),
 *          n>=0, with itself.
 * https://oeis.org/A073376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n-k+6,6) * binomial(n-k,k) *
 *            2^k
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 18, u64 n <= 43, u128 n <= 101,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sequence using the binomial helper functions. */
#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    seq_t result = 0;          /* The accumulated sum */
    seq_t pow2   = 1;          /* 2^k, starts with 2^0 */

    idx_t max_k = n >> 1;      /* floor(n/2) */

    for (idx_t k = 0; k <= max_k; ++k) {
        /* binom(n-k+6, 6) is always defined because k <= n/2 <= n */
        seq_t t1 = binom(n - k + 6, 6);

        /* binom(n-k, k) – if k > n-k it returns 0 (handled by binom) */
        seq_t t2 = binom(n - k, k);

        /* Multiply the terms together, then add to result */
        seq_t term = t1 * t2;
        term *= pow2;          /* multiply by 2^k */
        result += term;

        /* Update 2^k for next iteration */
        pow2 *= 2;
    }

    return result;
}
