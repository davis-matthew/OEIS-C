/*
 * A041549  Denominators of continued fraction convergents to sqrt(292).
 * https://oeis.org/A041549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n‑th convergent of the continued *
 *            fraction expansion of √292. The denominator is computed by the *
 *            standard recurrence * * q_{-2}=1, q_{-1}=0, * q_k = a_k·q_{k-1}
 *            + q_{k-2} * * where the partial quotients a_k are produced by
 *            the algorithm for the * continued fraction of...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    const idx_t D = 292;          /* the radicand */
    const idx_t a0 = 17;          /* floor(sqrt(292)) */

    /* Continued‑fraction state variables (all fit in uint64_t) */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    /* Recurrence state for the denominators q_k */
    seq_t q_prev_prev = 1;  /* q_{-2} */
    seq_t q_prev = 0;       /* q_{-1} */
    seq_t q_curr;

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute q_k = a_k · q_{k-1} + q_{k-2} */
        q_curr = (seq_t)a * q_prev + q_prev_prev;

        if (k == n) {
            /* We have the desired term; no need to update state further */
            return q_curr;
        }

        /* Generate next partial quotient a_{k+1} */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* Shift recurrence state for next iteration */
        q_prev_prev = q_prev;
        q_prev = q_curr;

        /* Advance continued‑fraction state */
        m = m_next;
        d = d_next;
        a = a_next;
    }

    /* The loop always returns; this point is unreachable. */
    return 0;
}
