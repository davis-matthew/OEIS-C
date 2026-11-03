/*
 * A062989  a(n) = binomial(n+6, 6) - n - 1.
 * https://oeis.org/A062989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+6,6) - n - 1
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 117, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 52 terms through n = 997 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t res = binomial(n + 6, 6);
    res -= (seq_t)n;
    res -= 1;
    return res;
}
