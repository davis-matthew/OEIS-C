/*
 * A220088  a(n) = 2^n - 81.
 * https://oeis.org/A220088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n - 81
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    return pow2(n) - (seq_t)81;
}
