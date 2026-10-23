/*
 * A107378  Expansion of x*(1-3*x-2*x^2)/(1-4*x+4*x^3+x^4).
 * https://oeis.org/A107378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(n)=4*a(n-1) - 4*a(n-3) -
 *            a(n-4) (n≥4)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle initial terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;

    /* Recursive computation using only the last four values */
    seq_t a0 = (seq_t)0;     /* a(n-4) */
    seq_t a1 = (seq_t)1;     /* a(n-3) */
    seq_t a2 = (seq_t)1;     /* a(n-2) */
    seq_t a3 = (seq_t)2;     /* a(n-1) */
    seq_t an  = a3;          /* placeholder for a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        an = (a3 * (seq_t)4) - (a1 * (seq_t)4) - a0;
        /* Shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
