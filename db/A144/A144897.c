/*
 * A144897  Expansion of x/(1 - 4*x + 6*x^2 - 5*x^3 + 4*x^4 - 3*x^5).
 * https://oeis.org/A144897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 36, u64 n <= 72, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A144897: Expansion of x/(1 - 4*x + 6*x^2 - 5*x^3 + 4*x^4 - 3*x^5)
   Recurrence (n ≥ 5): a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 4*a(n-4) + 3*a(n-5)
   Initial terms: a(0)=0, a(1)=1, a(2)=4, a(3)=10, a(4)=21
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first five terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)10;
    if (n == 4) return (seq_t)21;

    /* We only need the five most recent terms to compute the next one. */
    seq_t t0 = (seq_t)0;   /* a(n-5) */
    seq_t t1 = (seq_t)1;   /* a(n-4) */
    seq_t t2 = (seq_t)4;   /* a(n-3) */
    seq_t t3 = (seq_t)10;  /* a(n-2) */
    seq_t t4 = (seq_t)21;  /* a(n-1) */
    seq_t tn;

    for (idx_t i = 5; i <= n; ++i)
    {
        tn = (4 * t4) - (6 * t3) + (5 * t2) - (4 * t1) + (3 * t0);
        /* Shift the window of five previous terms for the next iteration. */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = tn;
    }
    return tn;
}
