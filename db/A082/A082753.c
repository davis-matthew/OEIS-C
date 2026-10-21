/*
 * A082753  Duplicate of A000337.
 * https://oeis.org/A082753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (n-2)*2^(n-1)+1
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 15, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t t = pow2(n - 1);
    return (sn - 2) * t + 1;
}
