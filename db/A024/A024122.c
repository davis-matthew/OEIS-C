/*
 * A024122  a(n) = 10^n - n^8.
 * https://oeis.org/A024122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 10^n - n^8
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 9, i64 n <= 18, i128 n <= 38,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n) {
    // Compute 10^n
    seq_t pow10 = pow_int((seq_t)10, n);
    // Compute n^8
    seq_t pow_n8 = pow_int((seq_t)n, (idx_t)8);
    // Return the difference
    return pow10 - pow_n8;
}
