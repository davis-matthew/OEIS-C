/*
 * A169960  a(n) = binomial(11*n,n).
 * https://oeis.org/A169960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(11*n, n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 196 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    return binomial(11 * n, n);
}
