/*
 * A119553  a(n) = binomial(n^2 * binomial(2*n,n), n).
 * https://oeis.org/A119553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n^2 * binomial(2*n,n), n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 39 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Multiplicative binomial coefficient for an arbitrary-precision N and
 * small k.  The division is exact at every step. */
static seq_t binom_from_N(seq_t N, idx_t k)
{
    if (k == 0) return (seq_t)1;
    if (N == (seq_t)0) return (seq_t)0; /* k>0 */

    seq_t result = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
    {
        seq_t term = N - (seq_t)(k - i); /* N - (k-i) = N - k + i */
        result = (result * term) / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute n^2 as seq_t */
    seq_t n_sq = (seq_t)n * (seq_t)n;

    /* Compute binomial(2n,n) using the provided helper that works for
     * idx_t arguments.  For n up to 10000 this stays within idx_t. */
    seq_t binom_2n_n = binomial((idx_t)(2 * n), n);

    /* N = n^2 * binom(2n,n) */
    seq_t N = n_sq * binom_2n_n;

    /* a(n) = binomial(N, n) */
    return binom_from_N(N, n);
}
