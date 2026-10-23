/*
 * A041432  Numerators of continued fraction convergents to sqrt(232).
 * https://oeis.org/A041432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth numerator of the continued‑fraction convergents to sqrt(232).
   The recurrence for the convergents is
     p_{-1}=1,  p_0=a0
     p_n  = a_n * p_{n-1} + p_{n-2}
   where a0 = floor(sqrt(232)) = 15 and the a_n (n≥1) are obtained from
   the standard algorithm for expanding sqrt(232) into a continued fraction. */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t D  = 232ULL;       /* The radicand */
    const uint64_t a0 = 15ULL;        /* floor(sqrt(232)) */

    if (n == 0) {
        return (seq_t)a0;
    }

    /* Variables for the continued‑fraction expansion of sqrt(232) */
    uint64_t m = 0;          /* m_{k-1} */
    uint64_t d = 1;          /* d_{k-1} */
    uint64_t a_prev = a0;    /* a_{k-1} */

    /* Recurrence for the convergent numerators */
    seq_t p_prevprev = (seq_t)1;  /* p_{-1} */
    seq_t p_prev     = (seq_t)a0; /* p_0 */
    seq_t p_curr;

    for (idx_t i = 1; i <= n; ++i) {
        /* next continued‑fraction coefficient a_i */
        m = d * a_prev - m;
        d = (D - m * m) / d;
        uint64_t a_i = (a0 + m) / d;

        /* recurrence: p_i = a_i * p_{i-1} + p_{i-2} */
        p_curr = (seq_t)a_i * p_prev + p_prevprev;

        /* shift indices for next iteration */
        p_prevprev = p_prev;
        p_prev     = p_curr;
        a_prev     = a_i;
    }

    return p_curr;
}
