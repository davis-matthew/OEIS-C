/*
 * A201686  a(n) = binomial(n, [n/2]) - 2.
 * https://oeis.org/A201686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = binomial(n, floor(n/2)) - 2
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 33, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 40 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t val = binomial(n, n / 2);
    return val - (seq_t)2;
}
