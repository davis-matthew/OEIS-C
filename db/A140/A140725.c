/*
 * A140725  Inverse binomial transform of (0 followed by A037481).
 * https://oeis.org/A140725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=4; a(n)=a(n-1)+6*a(n-2) for n>2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the recurrence a(n) = a(n-1) + 6*a(n-2) with the given base terms.  */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Handle the first three base cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    /* For n >= 3 we use the recurrence.  */
    /* prev2 holds a(n-2), prev1 holds a(n-1).  */
    seq_t prev2 = (seq_t)1;    /* a(1) */
    seq_t prev1 = (seq_t)4;    /* a(2) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        curr = prev1 + mul_small(prev2, 6); /* a(i) = a(i-1) + 6*a(i-2) */
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
