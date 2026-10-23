/*
 * A159561  Numerator of Hermite(n, 11/18).
 * https://oeis.org/A159561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 9^n * Hermite(n, 11/18) Recurrence: a(0)=1, a(1)=11,
 *            a(n)=11*a(n-1)-162*(n-1)*a(n-2) for n≥2
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    seq_t a_prev_prev = (seq_t)1;   /* a(0) */
    seq_t a_prev      = (seq_t)11;  /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t mult = ((seq_t)162) * ((seq_t)(i - 1));
        a_curr = (seq_t)(11 * a_prev - mult * a_prev_prev);
        a_prev_prev = a_prev;
        a_prev = a_curr;
    }
    return a_curr;
}
