/*
 * A042175  Denominators of continued fraction convergents to sqrt(612).
 * https://oeis.org/A042175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   denominators of continued fraction convergents to sqrt(612)
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 23, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Constants for the continued fraction of sqrt(612) */
    const seq_t D   = 612;   /* D = 612 */
    const seq_t a0  = 24;    /* a0 = floor(sqrt(612)) */

    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;   /* current a_k */

    /* Recurrence for denominators q_k */
    seq_t q_prev2 = 1;  /* q_{-2} */
    seq_t q_prev1 = 0;  /* q_{-1} */

    for (idx_t k = 0; k <= n; ++k) {
        /* q_k = a_k * q_{k-1} + q_{k-2} */
        seq_t q = a * q_prev1 + q_prev2;
        if (k == n) return q;

        /* shift previous denominators */
        q_prev2 = q_prev1;
        q_prev1 = q;

        /* compute next a_{k+1} via the standard continued‑fraction recurrence */
        seq_t m_next = d * a - m;
        seq_t d_next = (D - m_next * m_next) / d;          /* division is exact */
        seq_t a_next = (a0 + m_next) / d_next;             /* floor division */

        m = m_next;
        d = d_next;
        a = a_next;
    }
    /* This point is never reached */
    return 0;
}
