/*
 * A123877  Expansion of (1+2*x)/(1+3*x+3*x^2).
 * https://oeis.org/A123877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) satisfies (1+3x+3x^2) * A(x) = 1+2x Recurrence: a(0)=1,
 *            a(1)=-1, a(n) = -(3*a(n-1)+3*a(n-2)) for n≥2
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 39, i64 n <= 80, i128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    seq_t prev2 = (seq_t)1;    /* a(0) */
    seq_t prev1 = (seq_t)-1;   /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = -( (seq_t)3 * prev1 + (seq_t)3 * prev2 );
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
