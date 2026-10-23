/*
 * A042755  Denominators of continued fraction convergents to sqrt(908).
 * https://oeis.org/A042755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS entry A042755: Denominators of continued‑fraction convergents to sqrt(908).
 * Computed as q(n) where q(-2)=1, q(-1)=0 and
 *   q(k) = a(k)*q(k-1) + q(k-2)
 * with continued‑fraction coefficients
 *   a(0)=30, and for k>0,
 *   a(k) = {7,1,1,14,1,1,7,60} repeating.
 * Results: a(0)=1, a(1)=7, …
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* periodic part of the continued fraction of sqrt(908) */
    static const seq_t cycle[8] = {
        (seq_t)7,  (seq_t)1,  (seq_t)1,  (seq_t)14,
        (seq_t)1,  (seq_t)1,  (seq_t)7,  (seq_t)60
    };

    /* recurrence initial values: q(-2)=1, q(-1)=0 */
    seq_t q_prevprev = (seq_t)1; /* q(-2) */
    seq_t q_prev     = (seq_t)0; /* q(-1) */
    seq_t q_curr;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t a_k = (k == 0) ? (seq_t)30 :
                    cycle[(k - 1) % 8];  /* periodic cycle for k>0 */
        q_curr = a_k * q_prev + q_prevprev;
        q_prevprev = q_prev;
        q_prev = q_curr;
    }
    return q_curr;   /* q(n) is the required denominator */
}
