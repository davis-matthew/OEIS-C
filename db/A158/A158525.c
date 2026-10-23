/*
 * A158525  Number of connected spanning subgraphs and number of forests of
 *          the wheel graph W_n.
 * https://oeis.org/A158525
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + 2*a(n-3) with initial values
 *            a(4)=38, a(5)=134, a(6)=462 (derived from the generating
 *            function of A158525)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th term of OEIS sequence A158525.
   Works for any idx_t >= 4; returns 0 for n < 4. */
seq_t a(idx_t n)
{
    if (n < 4) return 0;

    /* Base values */
    const seq_t a4 = 38;
    const seq_t a5 = 134;
    const seq_t a6 = 462;

    if (n == 4) return a4;
    if (n == 5) return a5;
    if (n == 6) return a6;

    /* Iteratively apply the recurrence for n >= 7 */
    seq_t prev3 = a4;   /* a(i-3) */
    seq_t prev2 = a5;   /* a(i-2) */
    seq_t prev1 = a6;   /* a(i-1) */
    seq_t cur   = 0;    /* a(i) */

    for (idx_t i = 7; i <= n; ++i)
    {
        cur   = prev1 * 5 - prev2 * 6 + prev3 * 2;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;   /* a(n) */
}
