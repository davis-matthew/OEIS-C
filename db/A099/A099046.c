/*
 * A099046  a(n) = (4*0^n + 5^n*binomial(2*n,n))/5.
 * https://oeis.org/A099046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5^n * binomial(2*n,n) + 4*0^n)/5 For n ≥ 1, 0^n = 0, so
 *            a(n) = 5^(n-1) * binomial(2*n,n) a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1. */
    if (n == 0u)
        return (seq_t)1;

    /* Compute the central binomial coefficient C(2*n, n).  */
    seq_t comb = binomial(2*n, n);

    /* Compute 5^(n-1). */
    seq_t pow5 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        pow5 *= (seq_t)5;

    /* a(n) = 5^(n-1) * C(2*n, n).  */
    return pow5 * comb;
}
