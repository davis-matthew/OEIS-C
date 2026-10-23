/*
 * A106438  G.f.: x(2-5x-2x^2)/(1-6x+9x^2-x^4).
 * https://oeis.org/A106438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) + a(n-4) (for n ≥ 4) with initial
 *            values a(0)=0, a(1)=2, a(2)=7, a(3)=22
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 2;
    if (n == 2) return 7;
    if (n == 3) return 22;

    seq_t a0 = 0;   /* a(n-3) */
    seq_t a1 = 2;   /* a(n-2) */
    seq_t a2 = 7;   /* a(n-1) */
    seq_t a3 = 22;  /* current a(n)   */
    seq_t a_next;

    /* iterate from 4 up to n inclusive */
    for (idx_t i = 4; i <= n; ++i)
    {
        a_next = 6 * a3 - 9 * a2 + a0;
        /* shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a_next;
    }
    return a3;
}
