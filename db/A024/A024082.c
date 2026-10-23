/*
 * A024082  a(n) = 7^n - n^7.
 * https://oeis.org/A024082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 7^n - n^7
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 11, i64 n <= 22, i128 n <= 45,
 *            big any n
 * Helpers:   pow_uint_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint_fast.h"

seq_t a(idx_t n) {
    /* 7^n */
    seq_t pow7 = pow_uint_fast(7, n);

    /* n^7  (small constant exponent, just multiply n seven times) */
    seq_t pown7 = (seq_t)n;
    for (int i = 1; i < 7; ++i) {
        pown7 *= (seq_t)n;
    }

    /* a(n) = 7^n - n^7 */
    return pow7 - pown7;
}
