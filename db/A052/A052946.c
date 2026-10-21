/*
 * A052946  Expansion of (1-x)^2/(1-3*x+2*x^3-x^4).
 * https://oeis.org/A052946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A052946: Expansion of (1-x)^2/(1-3*x+2*x^3-x^4)
   Recurrence: a(n) = 3*a(n-1) - 2*a(n-3) + a(n-4)
   Initial values: a(0)=1, a(1)=1, a(2)=4, a(3)=10 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the four base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)10;

    /* Iterate from the fourth term up to the requested n.
       Only the last four terms are needed at any time. */
    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)4;   /* a(n-2) */
    seq_t a3 = (seq_t)10;  /* a(n-1) */
    seq_t ai;              /* a(n) */

    for (idx_t i = 4; i <= n; ++i)
    {
        ai = (seq_t)3 * a3 - (seq_t)2 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return a3;
}
