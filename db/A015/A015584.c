/*
 * A015584  Expansion of g.f. x/(1 - 9*x - 8*x^2).
 * https://oeis.org/A015584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) + 8*a(n-2); a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    seq_t prev2 = (seq_t)0;          /* a(0) */
    seq_t prev1 = (seq_t)1;          /* a(1) */
    seq_t curr;
    idx_t i;

    for (i = 2; i <= n; ++i) {
        curr = 9 * prev1 + 8 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;                    /* a(n) */
}
