/*
 * A024124  a(n) = 10^n - n^10.
 * https://oeis.org/A024124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 10^n - n^10
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 8, i64 n <= 18, i128 n <= 38,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Compute 10^n */
    seq_t pow10 = big_pow((seq_t)10, (seq_t)n);
    /* Compute n^10 */
    seq_t pow_n10 = big_pow((seq_t)n, (seq_t)10);
    return pow10 - pow_n10;
}
