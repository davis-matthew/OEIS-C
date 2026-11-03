/*
 * A000894  a(n) = (2*n)!*(2*n+1)! / ((n+1)! * n!^3).
 * https://oeis.org/A000894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2*n+1,n) * C(2*n,n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 171 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t c1 = binomial(2*n + 1, n);
    seq_t c2 = binomial(2*n, n);
    return c1 * c2;
}
