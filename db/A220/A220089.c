/*
 * A220089  a(n) = 2^n - 243.
 * https://oeis.org/A220089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2^n - 243
 * Range:     i8 none, i16 n <= 15, i32 n <= 31, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n and subtract 243. */
    return pow2(n) - (seq_t)243;
}
