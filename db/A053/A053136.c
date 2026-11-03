/*
 * A053136  Binomial coefficients C(2*n+7,7).
 * https://oeis.org/A053136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n+7,7)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 38, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 186 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t N = n + n + 7;
    return binomial(N, 7);
}
