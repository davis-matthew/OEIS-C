/*
 * A042818  Numerators of continued fraction convergents to sqrt(940).
 * https://oeis.org/A042818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042818: Numerators of continued‑fraction convergents to sqrt(940)
   a(0) = 30
   Recurrence:
     p_{-2} = 0,  p_{-1} = 1
     p_n   = a_n * p_{n-1} + p_{n-2}
   where a_0 = 30 and for n ≥ 1 the partial quotients repeat
   1, 1, 1, 14, 1, 1, 1, 60. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial partial quotient */
    const seq_t a0 = 30;

    /* Periodic part of the continued fraction for sqrt(940) */
    static const seq_t cf[8] = { 1, 1, 1, 14, 1, 1, 1, 60 };
    const unsigned int period = 8;

    seq_t p_minus2 = 0;   /* p_{-2} */
    seq_t p_minus1 = 1;   /* p_{-1} */
    seq_t p_curr   = 0;

    /* Compute p_n iteratively.  The loop runs (n+1) times, so that
       after the last iteration p_curr equals p_n. */
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t term = (i == 0) ? a0 : cf[(i - 1) % period];
        p_curr = term * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = p_curr;
    }

    return p_curr;
}
