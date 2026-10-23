/*
 * A042215  Denominators of continued fraction convergents to sqrt(633).
 * https://oeis.org/A042215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent of the continued
 *            fraction for √633
 * Range:     u8 none, u16 n <= 11, u32 n <= 18, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Square‑free integer D = 633, a0 = ⌊√D⌋. */
    const seq_t D   = (seq_t)633;
    const seq_t a0  = floor_sqrt(D);

    /* Initial values for the Pell‑type recurrence:  m₀ = 0, d₀ = 1. */
    seq_t m   = (seq_t)0;   /* m₀ */
    seq_t d   = (seq_t)1;   /* d₀ */
    seq_t a_k = a0;         /* a₀ */

    /* Convergents denominators:  q_{-2} = 1, q_{-1} = 0. */
    seq_t q_prevprev = (seq_t)1; /* q_{-2} */
    seq_t q_prev     = (seq_t)0; /* q_{-1} */
    seq_t q_curr;

    /* Iterate through the continued‑fraction terms until the n‑th convergent. */
    for (idx_t k = 0; k <= n; ++k) {
        /* q_k = a_k·q_{k-1} + q_{k-2} */
        q_curr = a_k * q_prev + q_prevprev;

        /* If we have reached the requested index, return the denominator. */
        if (k == n) {
            return q_curr;
        }

        /* Shift the denominator recurrence for the next step. */
        q_prevprev = q_prev;
        q_prev     = q_curr;

        /* Compute the next triple (m, d, a) for the continued fraction of √D. */
        seq_t next_m = d * a_k - m;
        seq_t next_d = (D - next_m * next_m) / d;   /* exact division */
        seq_t next_a = (a0 + next_m) / next_d;      /* floor((a0 + m_next) / d_next) */

        m = next_m;
        d = next_d;
        a_k = next_a;
    }

    /* The loop always returns before reaching this point, but return the last
       computed value as a safety net. */
    return q_curr;
}
