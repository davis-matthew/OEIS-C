/*
 * A165470  a(0)=1, a(1)=5, a(n) = 20*a(n-2) - a(n-1).
 * https://oeis.org/A165470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 14, i64 n <= 28, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=1, a(1)=5, a(n)=20*a(n-2)-a(n-1)   (n>=2)
 * (equivalently a(n) = (10*4^n-(-5)^n)/9, but the recurrence is used
 *  to avoid intermediate overflow).
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;
    if (n == 1U)
        return (seq_t)5;

    seq_t a_prev_prev = (seq_t)1;  /* a(0) */
    seq_t a_prev      = (seq_t)5;  /* a(1) */
    seq_t a_cur;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 20*a(i-2) - a(i-1) */
        a_cur = ((seq_t)20 * a_prev_prev) - a_prev;
        a_prev_prev = a_prev;
        a_prev = a_cur;
    }
    return a_cur;
}
