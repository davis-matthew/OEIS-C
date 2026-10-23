/*
 * A028014  Expansion of 1/((1-2x)(1-7x)(1-11x)(1-12x)).
 * https://oeis.org/A028014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 32·a(n-1) – 353·a(n-2) + 1510·a(n-3) – 1848·a(n-4)
   with a(0)=1, a(1)=32, a(2)=671, a(3)=11686. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Small base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)32;
    if (n == 2) return (seq_t)671;
    if (n == 3) return (seq_t)11686;

    /* Constants in seq_t form */
    const seq_t S1 = (seq_t)32;
    const seq_t S2 = (seq_t)353;
    const seq_t S3 = (seq_t)1510;
    const seq_t S4 = (seq_t)1848;

    /* Initialize first four terms */
    seq_t a0 = (seq_t)1;    /* a(0) */
    seq_t a1 = (seq_t)32;   /* a(1) */
    seq_t a2 = (seq_t)671;  /* a(2) */
    seq_t a3 = (seq_t)11686;/* a(3) */
    seq_t an;               /* a(n) being computed */

    /* Iterate up to desired n */
    for (idx_t i = 4; i <= n; ++i) {
        an = S1 * a3 - S2 * a2 + S3 * a1 - S4 * a0;
        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return a3;
}
