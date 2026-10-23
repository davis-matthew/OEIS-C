/*
 * A133666  a(n) = a(n-1) - 16*a(n-2), a(0)=1, a(1)=4.
 * https://oeis.org/A133666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 15, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) - 16*a(n-2), a(0)=1, a(1)=4 */
#include <stddef.h>   /* for size_t if needed */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)4;   /* a(1) */
    seq_t curr;
    const seq_t SIXTEEN = (seq_t)16;

    for (idx_t i = 2; i <= n; ++i) {
        curr = prev1 - prev2 * SIXTEEN;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
