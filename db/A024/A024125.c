/*
 * A024125  a(n) = 10^n - n^11.
 * https://oeis.org/A024125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 10^n - n^11
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 7, i64 n <= 18, i128 n <= 38,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    seq_t pow10 = seq_pow((seq_t)10, n);
    seq_t pown  = seq_pow((seq_t)n, 11);
    return pow10 - pown;
}
