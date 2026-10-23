/*
 * A122009  G.f. x*(1-11*x+6*x^2)/(1-12*x+15*x^2-2*x^3).
 * https://oeis.org/A122009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 15*a(n-2) + 2*a(n-3) with a(1) = 1, a(2) = 1,
 *            a(3) = 3 (OEIS A122009)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, used by idx_t */

seq_t a(idx_t n)
{
    /* undefined for n < 1, but we return 0 to avoid UB */
    if (n < 1)
        return (seq_t)0;

    /* base terms */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)3;

    /* Recurrence using only three previous values */
    seq_t prev3 = (seq_t)1;  /* a(n-3)  */
    seq_t prev2 = (seq_t)1;  /* a(n-2)  */
    seq_t prev1 = (seq_t)3;  /* a(n-1)  */
    seq_t curr;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* compute a(i) = 12*a(i-1) - 15*a(i-2) + 2*a(i-3) */
        curr = (seq_t)12 * prev1 + (seq_t)2 * prev3;
        curr -= (seq_t)15 * prev2;

        /* shift for next step */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
