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
 * Validated: standard, pass, 35 terms through n = 99 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    seq_t fac = factorial(n + 2ULL);
    seq_t bin = binomial(n + 6ULL, 6ULL);
    return (fac / 2) * bin;
}
