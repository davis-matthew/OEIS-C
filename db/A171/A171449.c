/*
 * A171449  Powers of 2 (A000079) with 1 changed to -1.
 * https://oeis.org/A171449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = -1 for n == 0 a(n) = 2^n for n >= 1 (powers of 2 with the
 *            0th term replaced by -1)
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    return (n == 0) ? (seq_t)-1 : pow2(n);
}
