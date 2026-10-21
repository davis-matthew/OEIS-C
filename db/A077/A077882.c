/*
 * A077882  Expansion of x/((1-x)*(1-x^2-2*x^3)).
 * https://oeis.org/A077882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) – 2*a(n-4) Initial values:
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=2
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is fully self‑contained and uses only standard C11. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;

    /* Iteratively compute the recurrence for n ≥ 4. */
    seq_t prev4 = (seq_t)0; /* a(n-4) */
    seq_t prev3 = (seq_t)1; /* a(n-3) */
    seq_t prev2 = (seq_t)1; /* a(n-2) */
    seq_t prev1 = (seq_t)2; /* a(n-1) */
    seq_t cur   = 0;

    for (idx_t i = 4; i <= n; ++i)
    {
        cur = prev1 + prev2 + prev3 - (seq_t)2 * prev4;
        /* Shift windows for next iteration. */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
