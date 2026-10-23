/*
 * A162852  a(n) = 3*a(n-2) for n > 2; a(1) = 3, a(2) = -1.
 * https://oeis.org/A162852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 18, i32 n <= 38, i64 n <= 78, i128 n <= 160,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 3; a(2) = -1; a(n) = 3*a(n-2) for n > 2. */
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    if (n == (idx_t)1) {
        return (seq_t)3;
    }
    if (n == (idx_t)2) {
        return (seq_t)-1;
    }
    if (n & (idx_t)1) {
        /* n is odd: a(n) = 3^((n+1)/2) */
        idx_t exp = (n + (idx_t)1) / (idx_t)2;
        return pow3(exp);
    } else {
        /* n is even: a(n) = -3^((n/2)-1) */
        idx_t exp = n / (idx_t)2 - (idx_t)1;
        seq_t val = pow3(exp);
        return -val;
    }
}
