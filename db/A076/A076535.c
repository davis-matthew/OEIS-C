/*
 * A076535  a(n) = A064405 (2^m+n) - 2^m (for m large enough this difference
 *          appears to be constant).
 * https://oeis.org/A076535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n + 1 - 4*A001316(n). This can be computed with the
 *            recurrence: a(0) = -3 a(2n) = a(n) + n a(2n+1) = 2*a(n)
 * Range:     i8 n <= 62, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)-3;
    if ((n & 1) == 0) {            /* even: a(2n) = a(n) + n */
        return a(n >> 1) + (seq_t)(n >> 1);
    } else {                       /* odd:  a(2n+1) = 2*a(n) */
        return (seq_t)2 * a(n >> 1);
    }
}
