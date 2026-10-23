/*
 * A192377  Coefficient of x in the reduction by x^2->x+2 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + 6*a(n-2) + 2*a(n-3) - a(n-4),
   a(1)=0, a(2)=2, a(3)=4, a(4)=20.  (OEIS A192377) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the small indices directly.  The caller may give any n ≥ 1. */
    if (n == 0) return (seq_t)0;          /* undefined, but harmless */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)20;

    /* Roll the recurrence using four variables. */
    seq_t a1 = (seq_t)0;   /* a(1) */
    seq_t a2 = (seq_t)2;   /* a(2) */
    seq_t a3 = (seq_t)4;   /* a(3) */
    seq_t a4 = (seq_t)20;  /* a(4) */
    seq_t an;              /* a(k) for current k */

    /* Compute from k = 5 up to n. */
    for (idx_t k = 5; k <= n; ++k) {
        an = (seq_t)2 * a4 + (seq_t)6 * a3 + (seq_t)2 * a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }

    return a4;   /* a(n) is now in a4 */
}
