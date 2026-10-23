/*
 * A042713  Denominators of continued fraction convergents to sqrt(886).
 * https://oeis.org/A042713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 21, u64 n <= 40, u128 n <= 82,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: denominators of continued‑fraction convergents to sqrt(886) */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    /* Parameters for sqrt(886) continued fraction */
    const idx_t D    = 886;                               /* the quadratic surd */
    const idx_t a0   = (idx_t)floor_sqrt((seq_t)D);       /* floor(sqrt(886)) */

    /* Convergents' denominator recurrence: q_{-2}=1, q_{-1}=0 */
    seq_t q_prevprev = (seq_t)1; /* q_{k-2} */
    seq_t q_prev     = (seq_t)0; /* q_{k-1} */

    /* Continued‑fraction state variables for computing partial quotients */
    idx_t m = 0;   /* m_k in the standard algorithm */
    idx_t d = 1;   /* d_k in the standard algorithm */
    idx_t a_k = a0;/* a_k (partial quotient) */

    for (idx_t i = 0; i <= n; ++i) {
        /* Current denominator */
        seq_t q_curr = (seq_t)a_k * q_prev + q_prevprev;

        if (i == n)
            return q_curr;

        /* Advance to next partial quotient */
        m = d * a_k - m;
        d = (D - m * m) / d;          /* integer division, exact */
        a_k = (a0 + m) / d;

        /* Shift for next iteration */
        q_prevprev = q_prev;
        q_prev     = q_curr;
    }

    /* Control should never reach here */
    return (seq_t)0;
}
