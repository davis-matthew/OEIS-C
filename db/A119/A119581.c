/*
 * A119581  a(n) = (2*n+n^2)*(binomial(2*n,n))/2.
 * https://oeis.org/A119581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+2)*binomial(2*n,n)/2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute n*(n+2) in seq_t to avoid overflow. */
    seq_t factor = (seq_t)n * ((seq_t)n + (seq_t)2);
    /* Central binomial coefficient C(2n,n). */
    seq_t binom = binomial((idx_t)(2 * n), n);
    /* Multiply and divide by 2. */
    seq_t prod   = factor * binom;
    seq_t result = prod / (seq_t)2;
    return result;
}
