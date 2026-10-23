/*
 * A042261  Denominators of continued fraction convergents to sqrt(656).
 * https://oeis.org/A042261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 29, u64 n <= 55, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) is the denominator of the n‑th convergent of the
   simple continued fraction for sqrt(656).
   The continued fraction of sqrt(656) has the form
      [25; 1,1,1,1,2,1,1,1,1,50, 1,1,1,1,2,1,1,1,1,50, …]
   where the period length is 10.  The recurrence for convergents
   (p[i],q[i]) is
      p[i] = a[i]·p[i‑1] + p[i‑2],
      q[i] = a[i]·q[i‑1] + q[i‑2],
   with p[‑2]=0, p[‑1]=1, q[‑2]=1, q[‑1]=0.  The requested term is q[n]. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Periodic coefficients after the initial term a0=25. */
    static const seq_t period_vals[10] = {1, 1, 1, 1, 2, 1, 1, 1, 1, 50};

    seq_t q_prevprev = 1;   /* q[-2] */
    seq_t q_prev     = 0;   /* q[-1] */
    seq_t q_curr     = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t coeff = (i == 0) ? (seq_t)25
                               : period_vals[(size_t)((i - 1) % 10)];
        q_curr = coeff * q_prev + q_prevprev;
        q_prevprev = q_prev;
        q_prev = q_curr;
    }
    return q_curr;
}
