/*
 * A200380  Expansion of e.g.f. exp(x+x^2-1/6*x^3).
 * https://oeis.org/A200380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 16, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=3;
   a(n)=a(n-1)+2*(n-1)*a(n-2)-((n-1)*(n-2)/2)*a(n-3) */

#include <stdint.h>

/* The function a is declared externally in the build environment.
   It takes an unsigned 64‑bit index and returns a seq_t value, which
   may be a native integer type or an arbitrary‑precision integer class. */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* Keep the three most recent values */
    seq_t a_nm3 = (seq_t)1;   /* a(0) */
    seq_t a_nm2 = (seq_t)1;   /* a(1) */
    seq_t a_nm1 = (seq_t)3;   /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t i_minus1 = (seq_t)(i - 1);
        seq_t i_minus2 = (seq_t)(i - 2);

        /* term2 = 2*(i-1)*a(i-2) */
        seq_t term2 = (seq_t)2 * i_minus1 * a_nm2;

        /* term3 = ((i-1)*(i-2)/2)*a(i-3) */
        seq_t mult = i_minus1 * i_minus2 / (seq_t)2;
        seq_t term3 = mult * a_nm3;

        /* a(i) = a(i-1) + term2 - term3 */
        seq_t a_i = a_nm1 + term2 - term3;

        /* shift for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_i;
    }

    return a_nm1;
}
