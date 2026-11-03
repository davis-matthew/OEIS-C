/*
 * A168616  a(n) = 2^n - 5.
 * https://oeis.org/A168616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n - 5
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 861 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t res = pow2(n); /* compute 2^n */
    res -= (seq_t)5;     /* subtract 5 */
    return res;
}
