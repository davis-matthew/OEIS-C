/*
 * A080206  a(1)=a(2)=a(3)=1; a(n)=|a(n-2)+a(n-3)|-2*a(n-1).
 * https://oeis.org/A080206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = |a(n-2)+a(n-3)| - 2*a(n-1) for n > 3 with initial values
 *            a(1) = a(2) = a(3) = 1 (a(4) = 0 according to the recurrence)
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 34, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   abs_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/abs_seq.h"

/* sequence term a(n), with n >= 1 */
seq_t a(idx_t n)
{
    if (n <= 0)   /* undefined below the offset, return 0 */
        return 0;

    if (n <= 3)   /* a(1)=a(2)=a(3)=1 */
        return (seq_t)1;

    /* iterate using three previous terms */
    seq_t a_prev3 = (seq_t)1;   /* a(n-3) */
    seq_t a_prev2 = (seq_t)1;   /* a(n-2) */
    seq_t a_prev1 = (seq_t)1;   /* a(n-1) */

    idx_t i;
    for (i = 4; i <= n; ++i) {
        seq_t next = abs_seq(a_prev2 + a_prev3) - (seq_t)2 * a_prev1;
        /* shift the window */
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = next;
    }
    return a_prev1;
}
