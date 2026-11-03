/*
 * A092813  Schmidt's problem sum for r = 3.
 * https://oeis.org/A092813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^3 * binomial(n+k,k)^3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 194 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t c1 = binomial(n, k);
        seq_t c2 = binomial(n + k, k);
        seq_t term = c1 * c1 * c1 * c2 * c2 * c2;
        sum += term;
    }
    return sum;
}
