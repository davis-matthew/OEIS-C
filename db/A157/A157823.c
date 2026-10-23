/*
 * A157823  a(n) = A156591(n) + A156591(n+1).
 * https://oeis.org/A157823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2^(n-1) for n >= 1, a(0) = -5
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined by a(0) = -5,  a(1) = -1, and a(n) = 2*a(n-1) for n > 1. */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)-5;
    }
    /* For n >= 1 compute -2^(n-1) */
    idx_t exp = n - 1;
    seq_t pow_val = pow2(exp);   /* 2^exp in seq_t */
    return -pow_val;
}
