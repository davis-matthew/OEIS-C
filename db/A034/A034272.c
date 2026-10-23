/*
 * A034272  a(n) = f(n,n+5) where f is given in A034261.
 * https://oeis.org/A034272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n+5, n+6)*(n^2+6*n+1)/(n+7)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute binomial(2*n+5, n+6) */
    seq_t bin = binomial((2 * n) + 5, n + 6);

    /* Compute (n^2 + 6*n + 1) */
    seq_t poly = (seq_t)n * (seq_t)n + (seq_t)6 * (seq_t)n + 1;

    /* Multiply and divide while staying in seq_t bounds */
    seq_t res = bin * poly;
    res /= (n + 7);

    return res;
}
