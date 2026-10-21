/*
 * A203073  G.f.: (4-4*x-20*x^2)/(1-19*x+2*x^2+78*x^3).
 * https://oeis.org/A203073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) for OEIS A203073.
   Generating function: (4-4*x-20*x^2)/(1-19*x+2*x^2+78*x^3).
   Recurrence:   a(0)=4, a(1)=72, a(2)=1340,
                 a(n) = 19*a(n-1) - 2*a(n-2) - 78*a(n-3)   for n ≥ 3.   */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)72;
    if (n == 2) return (seq_t)1340;

    seq_t a0 = (seq_t)4;          /* a(n-3) */
    seq_t a1 = (seq_t)72;         /* a(n-2) */
    seq_t a2 = (seq_t)1340;       /* a(n-1) */
    seq_t an = a2;                /* will hold a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)19 * a2            /* 19*a(n-1) */
            - (seq_t)2 * a1            /* -2*a(n-2) */
            - (seq_t)78 * a0;          /* -78*a(n-3) */

        a0 = a1;    /* shift window forward */
        a1 = a2;
        a2 = an;
    }

    return an;
}
