/*
 * A041108  Numerators of continued fraction convergents to sqrt(62).
 * https://oeis.org/A041108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041108: Numerators of continued fraction convergents to sqrt(62)
   Sequence defined by
       p[-2] = 0,  p[-1] = 1
       p[k] = a_k * p[k-1] + p[k-2]
   with a_0 = 7, and for k ≥ 1 the partial quotients repeat
       a_k ∈ {1, 6, 1, 14} .
   This function returns p[n]. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* periodic part of the continued fraction for sqrt(62) */
    const seq_t period[4] = {1, 6, 1, 14};

    seq_t p_prev2 = 0;  /* p[k-2] */
    seq_t p_prev1 = 1;  /* p[k-1] */
    seq_t p_curr  = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t coeff;
        if (k == 0)
            coeff = 7;                     /* a_0 */
        else
            coeff = period[(k - 1) % 4];    /* a_k for k ≥ 1 */

        p_curr = coeff * p_prev1 + p_prev2;   /* recurrence */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    return p_curr;   /* p[n] */
}
