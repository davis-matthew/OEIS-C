/*
 * A042771  Denominators of continued fraction convergents to sqrt(916).
 * https://oeis.org/A042771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 20, u64 n <= 40, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Denominators of continued‑fraction convergents to sqrt(916).
 * Computed via the standard recurrence
 *    q_k = a_k * q_{k-1} + q_{k-2},
 * where a_0 = ⌊√916⌋ = 30 and the subsequent coefficients a_k are those of
 * the periodic continued fraction expansion of √916. */
/* Note: this implementation does not use the long‑term linear recurrence
 * given for the sequence (a(n) = 11696402*a(n-14) - a(n-28)),
 * but generates the terms directly from the continued‑fraction
 * recurrence, which is faster and accurate for all n. */
#include <stdint.h>
#include <stddef.h>

/* Computes the n-th term of OEIS A042771. */
seq_t a(idx_t n)
{
    const seq_t D = (seq_t)916;   /* D in the quadratic surd sqrt(D) */
    const seq_t A0 = (seq_t)30;   /* floor(sqrt(916)) */

    /* Denominator recurrence: q_-2 = 1, q_-1 = 0 */
    seq_t q_nm2 = (seq_t)1;
    seq_t q_nm1 = (seq_t)0;
    seq_t q_k   = (seq_t)0;

    /* Continued‑fraction auxiliary variables initialized to the first term */
    seq_t m  = (seq_t)0; /* m_0 */
    seq_t d  = (seq_t)1; /* d_0 */
    seq_t a_k = A0;      /* a_0 = floor(sqrt(D)) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Compute the k-th denominator */
        q_k = a_k * q_nm1 + q_nm2;

        /* Shift indices for the next step */
        q_nm2 = q_nm1;
        q_nm1 = q_k;

        /* Generate the next continued‑fraction coefficient a_{k+1} */
        seq_t m_next = d * a_k - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (A0 + m_next) / d_next;

        m   = m_next;
        d   = d_next;
        a_k = a_next;
    }

    return q_k;
}
