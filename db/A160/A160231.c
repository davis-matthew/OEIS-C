/*
 * A160231  Numerator of Hermite(n, 4/29).
 * https://oeis.org/A160231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 4/29) Recurrence: a(0) = 1, a(1) = 8,
 *            a(n) = 8*a(n-1) - 1682*(n-1)*a(n-2) (n ≥ 2) This recurrence
 *            follows from H_n(x) = 2x H_{n-1}(x) - 2(n-1) H_{n-2}(x) with x =
 *            4/29, multiplied by 29^n
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the first two terms explicitly */
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)8;
    }

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)8;   /* a(1) */
    seq_t curr   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* term = 1682 * (i-1) * a(i-2) */
        seq_t term = (seq_t)1682 * (seq_t)(i - 1) * prev2;
        /* a(i) = 8 * a(i-1) - term */
        curr = (seq_t)8 * prev1 - term;

        /* slide the window */
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
