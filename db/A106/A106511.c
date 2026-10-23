/*
 * A106511  Expansion of g.f. (1+x)^2/((1 + x + x^2)*(1 + x - x^2)).
 * https://oeis.org/A106511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -2*a(n-1) - a(n-2) + a(n-4) Initial values: a(0) = 1,
 *            a(1) = a(2) = a(3) = 0 This implementation is iterative and uses
 *            only O(1) extra memory
 * Range:     i8 n <= 14, i16 n <= 25, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n <= 3) return (seq_t)0;

    /* four most recent terms: a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t prev4 = (seq_t)1;    /* a(0) */
    seq_t prev3 = (seq_t)0;    /* a(1) */
    seq_t prev2 = (seq_t)0;    /* a(2) */
    seq_t prev1 = (seq_t)0;    /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t ai = - (seq_t)2 * prev1 - prev2 + prev4;
        /* shift window */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = ai;
    }
    return prev1;
}
