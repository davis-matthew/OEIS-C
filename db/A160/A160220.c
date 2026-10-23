/*
 * A160220  Numerator of Hermite(n, 19/28).
 * https://oeis.org/A160220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A160220):
 *
 * a(n) = 14^n * Hermite(n, 19/28)
 * Using the physicist Hermite recurrence
 *   H_{n+1}(x)=2x H_n(x) - 2n H_{n-1}(x)
 * we obtain the integer recurrence
 *
 *   a(0) = 1
 *   a(1) = 19
 *   a(n+1) = 19 a(n) - 392 n a(n-1)   for n >= 1
 *
 * This implementation evaluates the recurrence iteratively.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)19;

    seq_t prevprev = (seq_t)1;   /* a(0) */
    seq_t prev     = (seq_t)19;  /* a(1) */
    seq_t curr	    = prev;     /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 19*a(i-1) - 392*(i-1)*a(i-2) */
        seq_t prod = (seq_t)392 * (idx_t)(i - 1);  /* 392*(i-1) */
        curr = prev * (seq_t)19 - prevprev * prod;
        prevprev = prev;
        prev = curr;
    }
    return prev;
}
