/*
 * A182104  Duplicate of A073587.
 * https://oeis.org/A182104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = a(n-1)*2^n + 1 with a(0) = 1
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 7, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n) {
    seq_t res = (seq_t)1;
    if (n == 0) return res;
    seq_t pow2 = ((seq_t)1) << 1; /* 2^1 */
    for (idx_t i = 1; i <= n; ++i) {
        res = res * pow2 + (seq_t)1;
        pow2 = pow2 << 1; /* 2^(i+1) for next iteration */
    }
    return res;
}
