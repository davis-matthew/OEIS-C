/*
 * A051745  a(n) = n*(n^4 + 10*n^3 + 35*n^2 + 50*n + 144)/120.
 * https://oeis.org/A051745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+4, 5) + n
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 218, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 951 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t b = binomial(n + 4, 5);
    return b + (seq_t)n;
}
