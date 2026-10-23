/*
 * A129937  a(n) = binomial(n, floor(n/2)) - n*(n - 1)/2.
 * https://oeis.org/A129937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = binomial(n, floor(n/2)) - n*(n-1)/2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t k = n / 2;                /* floor(n/2) */
    seq_t bin = binomial(n, k);     /* C(n, floor(n/2)) */
    seq_t tri = (seq_t)n * (seq_t)(n - 1) / 2; /* triangular number */
    return bin - tri;
}
