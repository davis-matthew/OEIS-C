/*
 * A214091  a(n) = 3^n - 2^(n+2).
 * https://oeis.org/A214091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3^n - 2^(n+2)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 19, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Compute 3^n and 2^(n+2) using the provided big_pow helper,
       then subtract to obtain the required term. */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n + (seq_t)2);
    return pow3 - pow2;
}
