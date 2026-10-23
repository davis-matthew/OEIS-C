/*
 * A102341  Areas of 'close-to-equilateral' integer triangles.
 * https://oeis.org/A102341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   By the empirical generating function * 12*x /
 *            ((x^2-14*x+1)*(x^2+4*x+1)) * the terms satisfy the linear
 *            recurrence * a(n) = 10*a(n-1) + 54*a(n-2) + 10*a(n-3) - a(n-4) *
 *            with initial values * a(1)=12, a(2)=120, a(3)=1848, a(4)=25080.
 *            * This recurrence requires only additions, subtractions and *
 *            mu...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle n < 1 gracefully – not part of the OEIS range */
    if (n == 0) return 0;

    /* Initial terms (index 1..4) */
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)120;
    if (n == 3) return (seq_t)1848;
    if (n == 4) return (seq_t)25080;

    /* Sliding window of the last four terms */
    seq_t a1 = (seq_t)12;    /* a(n-4) */
    seq_t a2 = (seq_t)120;   /* a(n-3) */
    seq_t a3 = (seq_t)1848;  /* a(n-2) */
    seq_t a4 = (seq_t)25080; /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 5; i <= n; ++i) {
        an = 10 * a4 + 54 * a3 + 10 * a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
