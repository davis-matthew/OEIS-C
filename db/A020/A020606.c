/*
 * A020606  Expansion of g.f. 1/((1-6*x)*(1-9*x)*(1-11*x)).
 * https://oeis.org/A020606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*a(n-1) - 219*a(n-2) + 594*a(n-3); a(0)=1, a(1)=26,
 *            a(2)=457
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1U;
    if (n == 1U) return (seq_t)26U;
    if (n == 2U) return (seq_t)457U;

    seq_t a0 = (seq_t)1U;      /* a(n-3) */
    seq_t a1 = (seq_t)26U;     /* a(n-2) */
    seq_t a2 = (seq_t)457U;    /* a(n-1) */
    seq_t a3;

    for (idx_t i = 3U; i <= n; ++i)
    {
        a3 = (seq_t)26U * a2 - (seq_t)219U * a1 + (seq_t)594U * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
