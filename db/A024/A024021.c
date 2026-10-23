/*
 * A024021  a(n) = 2^n-n^11.
 * https://oeis.org/A024021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n - n^11
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 7, i64 n <= 52, i128 n <= 127,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 2^n and n^11 using big_pow, then subtract. */
    seq_t pow2  = big_pow((seq_t)2, (seq_t)n);
    seq_t pow11 = big_pow((seq_t)n, (seq_t)11);
    return pow2 - pow11;
}
