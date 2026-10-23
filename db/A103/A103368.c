/*
 * A103368  Period 6: repeat [1, 1, -1, -1, 0, 0].
 * https://oeis.org/A103368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = period 6 pattern [1, 1, -1, -1, 0, 0] with a(0)=1
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t period[6] = {1, 1, -1, -1, 0, 0};

seq_t a(idx_t n) {
    /* n is unsigned, modulus 6 never exceeds 5, safe cast to int */
    return period[(int)(n % 6)];
}
