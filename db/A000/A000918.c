/*
 * A000918  a(n) = 2^n - 2.
 * https://oeis.org/A000918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n - 2
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 995 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    return pow2(n) - 2;
}
