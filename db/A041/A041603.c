/*
 * A041603  Denominators of continued fraction convergents to sqrt(319).
 * https://oeis.org/A041603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 38, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of convergents to sqrt(319).  The sequence a(n)
   is produced from the standard continued‑fraction recurrence
   q_k = a_k * q_{k‑1} + q_{k‑2} with q_{-2}=1, q_{-1}=0.
   The partial quotients a_k are obtained from the expansion of
   sqrt(319).  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constant terms of the continued‑fraction expansion of sqrt(319) */
    const idx_t D   = 319ULL;   /* the radicand */
    const idx_t a0  = 17ULL;    /* floor(sqrt(319)) */

    /* Initial values for the algorithm */
    idx_t m = 0;        /* previous m */
    idx_t d = 1;        /* previous d */
    idx_t a_k = a0;     /* current partial quotient a_k */

    seq_t q_prevprev = 1;  /* q_{-2} */
    seq_t q_prev     = 0;  /* q_{-1} */

    for (idx_t i = 0; i <= n; ++i) {
        /* Compute the current denominator q_i */
        seq_t q = a_k * q_prev + q_prevprev;
        if (i == n)
            return q;

        /* Prepare the next partial quotient a_{k+1} */
        idx_t m_new = d * a_k - m;
        idx_t d_new = (D - m_new * m_new) / d;
        idx_t a_next = (a0 + m_new) / d_new;

        /* Shift for the next iteration */
        m = m_new;
        d = d_new;
        a_k = a_next;
        q_prevprev = q_prev;
        q_prev = q;
    }

    /* Unreachable: the loop always returns. */
    return 0;
}
