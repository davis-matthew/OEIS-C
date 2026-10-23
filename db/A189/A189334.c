/*
 * A189334  Expansion of g.f. (1-6*x+x^2)/(1-10*x+5*x^2).
 * https://oeis.org/A189334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 5*a(n-2), a(0)=1, a(1)=4, a(2)=36
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h> /* for bool, if needed by compiler */

seq_t a(idx_t n)
{
    /* handle initial terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)36;

    /* a(1) and a(2) for recurrence */
    seq_t prev2 = (seq_t)4;   /* a(1) */
    seq_t prev1 = (seq_t)36;  /* a(2) */
    seq_t current = prev1;    /* will hold a(i) */

    idx_t i;
    for (i = 3; i <= n; ++i) {
        current = (seq_t)10 * prev1 - (seq_t)5 * prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return current;
}
