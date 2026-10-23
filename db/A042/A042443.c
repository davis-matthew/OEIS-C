/*
 * A042443  Denominators of continued fraction convergents to sqrt(749).
 * https://oeis.org/A042443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 22, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of the continued‑fraction convergents to sqrt(749).
   The continued fraction of sqrt(749) is
   [27; 2,1,2,1,1,4,2,1,1,13,10,1,6,1,10,13,1,1,2,4,1,1,2,1,2,54],
   where the 54 is 2·27.  The denominators q_n satisfy
   q_{-2}=1, q_{-1}=0, q_k = a_k · q_{k-1} + q_{k-2} (k≥0),
   where a_0=27 and for k>0 a_k is given by the periodic list above.
   a(n) = q_n.                                                    */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Return the first denominator explicitly. */
    if (n == 0)
        return (seq_t)1;     /* q_0 = 1 */

    /* Periodic part of the continued fraction after a_0. */
    static const seq_t period[26] = {
        2, 1, 2, 1, 1, 4, 2, 1, 1, 13, 10, 1,
        6, 1, 10, 13, 1, 1, 2, 4, 1, 1, 2, 1, 2, 54
    };
    const idx_t period_len = 26;          /* length of the period */

    /* Recurrence variables for numerators (p) and denominators (q). */
    seq_t p_prev2 = 0,    /* p_{-2} */
          p_prev1 = 1;    /* p_{-1} */
    seq_t q_prev2 = 1,    /* q_{-2} */
          q_prev1 = 0;    /* q_{-1} */

    /* Handle the a_0 term (27). */
    const seq_t a0 = 27;
    seq_t p = a0 * p_prev1 + p_prev2;     /* p_0 = 27 */
    seq_t q = a0 * q_prev1 + q_prev2;     /* q_0 = 1 */

    p_prev2 = p_prev1;  p_prev1 = p;
    q_prev2 = q_prev1;  q_prev1 = q;

    /* Compute subsequent convergents up to index n. */
    for (idx_t k = 1; k <= n; ++k) {
        /* Periodic coefficient a_k for k ≥ 1. */
        seq_t bk = period[(size_t)((k - 1) % period_len)];

        /* Recurrence step. */
        p = bk * p_prev1 + p_prev2;
        q = bk * q_prev1 + q_prev2;

        /* Shift for next iteration. */
        p_prev2 = p_prev1;  p_prev1 = p;
        q_prev2 = q_prev1;  q_prev1 = q;
    }

    return q;   /* q_n is the desired denominator. */
}
