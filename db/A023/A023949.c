/*
 * A023949  Expansion of g.f. 1/((1-x)*(1-6*x)*(1-7*x)*(1-8*x)).
 * https://oeis.org/A023949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1) - 167*a(n-2) + 482*a(n-3) - 336*a(n-4), with
 *            initial values a(0) = 1, a(1) = 22, a(2) = 317, a(3) = 3782.
 *            This implements OEIS sequence A023949
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)22;
    if (n == 2) return (seq_t)317;
    if (n == 3) return (seq_t)3782;

    /* Use a sliding window of the last four values. */
    seq_t a0 = (seq_t)1;      /* a(n-4) */
    seq_t a1 = (seq_t)22;     /* a(n-3) */
    seq_t a2 = (seq_t)317;    /* a(n-2) */
    seq_t a3 = (seq_t)3782;   /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        an = 22 * a3            /* 22*a(n-1) */
             - 167 * a2          /* -167*a(n-2) */
             + 482 * a1          /* +482*a(n-3) */
             - 336 * a0;         /* -336*a(n-4) */

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
