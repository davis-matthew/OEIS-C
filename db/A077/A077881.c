/*
 * A077881  Expansion of (1-x)^(-1)/(1-2*x^2+2*x^3).
 * https://oeis.org/A077881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - 4*a(n-3) + 2*a(n-4) Initial values:
 *            a(0)=1, a(1)=1, a(2)=3, a(3)=1 Generated from
 *            G(x)=1/((1-x)*(1-2x^2+2x^3))
 * Range:     i8 n <= 10, i16 n <= 20, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0u) return 1;
    if (n == 1u) return 1;
    if (n == 2u) return 3;
    if (n == 3u) return 1;

    /* Recurrence uses the four previous values.  All arithmetic
       is performed in seq_t, which may be any signed integer type
       or arbitrary‑precision. */
    seq_t a0 = 1;   /* a(n-4) */
    seq_t a1 = 1;   /* a(n-3) */
    seq_t a2 = 3;   /* a(n-2) */
    seq_t a3 = 1;   /* a(n-1) */
    seq_t an;

    for (idx_t i = 4u; i <= n; ++i) {
        an = a3 + 2 * a2 - 4 * a1 + 2 * a0;  /* a(i) */
        /* Shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
