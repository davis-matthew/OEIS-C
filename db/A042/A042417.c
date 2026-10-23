/*
 * A042417  Denominators of continued fraction convergents to sqrt(736).
 * https://oeis.org/A042417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(736). */
/* Recurrence: q0 = 1, q1 = 7; for k >= 2,
   qk = a_k * q_{k-1} + q_{k-2}, where a_k cycles through
   [7, 1, 2, 1, 2, 1, 7, 54] (period 8) for k >= 1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)7;
    }

    /* Periodic partial quotients starting with a1. */
    static const seq_t period[8] = {7, 1, 2, 1, 2, 1, 7, 54};

    seq_t q_prevprev = (seq_t)1; /* q0 */
    seq_t q_prev    = (seq_t)7;  /* q1 */
    seq_t q_curr;

    for (idx_t k = 2; k <= n; ++k) {
        seq_t acoeff = period[(k - 1) % 8];      /* a_k for k>=1 */
        q_curr = acoeff * q_prev + q_prevprev;   /* recurrence */
        q_prevprev = q_prev;
        q_prev = q_curr;
    }

    return q_prev;
}
