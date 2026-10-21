/*
 * A206145  Number of (n+1) X 4 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly three counterclockwise and three clockwise edge
 *          increases.
 * https://oeis.org/A206145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 15*a(n-2) + 6*a(n-3) with a(1) = 162, a(2) =
 *            1122, a(3) = 7812
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* the sequence is defined only for n >= 1 */
    if (n == 0) return (seq_t)0;

    /* initial values */
    if (n == 1) return (seq_t)162;
    if (n == 2) return (seq_t)1122;
    if (n == 3) return (seq_t)7812;

    /* iterative recurrence */
    seq_t prev3 = (seq_t)162;   /* a(1) */
    seq_t prev2 = (seq_t)1122;  /* a(2) */
    seq_t prev1 = (seq_t)7812;  /* a(3) */
    seq_t curr;

    for (idx_t i = 4; i <= n; ++i) {
        curr = (seq_t)9 * prev1 - (seq_t)15 * prev2 + (seq_t)6 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
