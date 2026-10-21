/*
 * A052622  E.g.f. (1-x^2)/(1-2x-x^2).
 * https://oeis.org/A052622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=8;
   for n ≥ 1:  a(n+2) = (2 + n² + 3n)·a(n) + (4 + 2n)·a(n+1)   (OEIS A052622) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;

    /* start with a(1) and a(2) */
    seq_t a_prev = (seq_t)2;   /* a(n-1)  – a(1) */
    seq_t a_curr = (seq_t)8;   /* a(n)    – a(2) */

    /* compute a(n) by iterating the recurrence from n = 1 up to the target */
    for (idx_t i = 1; i <= n-2; ++i)
    {
        /* coefficients as defined by the recurrence */
        seq_t coeff1 = (seq_t)2 + (seq_t)i * (seq_t)i + (seq_t)3 * (seq_t)i;
        seq_t coeff2 = (seq_t)4 + (seq_t)2 * (seq_t)i;

        /* next term: a(i+2) = coeff1 * a_prev + coeff2 * a_curr */
        seq_t next = coeff1 * a_prev + coeff2 * a_curr;

        /* shift the window for the next iteration */
        a_prev = a_curr;
        a_curr = next;
    }

    return a_curr;   /* a(n) */
}
