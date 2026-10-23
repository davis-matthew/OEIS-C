/*
 * A162871  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=39, a(2)=1482, a(3)=55575 a(n) = 37*a(n-1) +
 *            37*a(n-2) - 703*a(n-3) for n >= 4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return the few base values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)39;
    if (n == 2) return (seq_t)1482;
    if (n == 3) return (seq_t)55575;

    /* Maintain the last three terms needed for the recurrence. */
    seq_t prev3 = (seq_t)39;      /* a(n-3) when n = 4          */
    seq_t prev2 = (seq_t)1482;    /* a(n-2) when n = 4          */
    seq_t prev1 = (seq_t)55575;   /* a(n-1) when n = 4          */
    seq_t curr  = 0;

    for (idx_t i = 4; i <= n; ++i) {
        curr = (37 * prev1 + 37 * prev2) - (703 * prev3);

        /* Shift the window for the next iteration. */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
