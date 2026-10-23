/*
 * A193659  Q-residue of the triangle A094727, where Q=Pascal's triangle. (See
 *          Comments.)
 * https://oeis.org/A193659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+5)*a(n-1) - 2*(2n+1)*a(n-2) + (5n-8)*a(n-3) -
 *            2*(n-3)*a(n-4) for n ≥ 4, with initial values: a(0)=1, a(1)=2,
 *            a(2)=8, a(3)=43 (equivalently:
 *            a(n)+(-n-5)a(n-1)+2(2n+1)a(n-2)+(-5n+8)a(n-3)+2(n-3)a(n-4)=0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the base cases explicitly */
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 8;
    if (n == 3) return 43;

    /* Sliding window for the last four terms
       prev[0] = a(n-4), prev[1] = a(n-3), prev[2] = a(n-2), prev[3] = a(n-1) */
    seq_t prev[4] = {1, 2, 8, 43};

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t sn = i;                                    /* n as seq_t */

        /* Numerator terms of the recurrence */
        seq_t term1 = (sn + 5) * prev[3]                /* (n+5)*a(n-1) */
                    + (5 * sn - 8) * prev[1];            /* +(5n-8)*a(n-3) */
        seq_t term2 = (2 * (2 * sn + 1)) * prev[2]     /* 2*(2n+1)*a(n-2) */
                    + (2 * (sn - 3)) * prev[0];         /* +2*(n-3)*a(n-4) */

        seq_t cur = term1 - term2;                     /* a(n) */

        /* Shift the window for the next iteration */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = cur;
    }

    return prev[3];   /* a(n) is the last computed term */
}
