/*
 * A062260  Third (unsigned) column sequence of triangle A062140 (generalized
 *          a=4 Laguerre).
 * https://oeis.org/A062260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)! * binomial(n+6,6) / 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t fac = factorial(n + 2ULL);
    seq_t bin = binomial(n + 6ULL, 6ULL);
    return (fac / 2) * bin;
}
