/*
 * A042690  Numerators of continued fraction convergents to sqrt(875).
 * https://oeis.org/A042690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=29, a(1)=30, a(n)=a(n-1)*c_n + a(n-2)
   where c_n are the continued‐fraction terms of sqrt(875). */

#include <stdint.h>

seq_t a(idx_t n) {
    const unsigned long long N   = 875ULL;   /* target number under sqrt */
    const unsigned long long a0  = 29ULL;    /* floor(sqrt(875)) */

    /* continued fraction state */
    unsigned long long m = 0;
    unsigned long long d = 1;
    unsigned long long a_term = a0;          /* a_k in the CF */

    /* convergent numerators (p_{-2}=0, p_{-1}=1) */
    seq_t p_prevprev = 0;
    seq_t p_prev    = 1;
    seq_t p = 0;

    for (idx_t i = 0;; ++i) {
        /* calculate current numerator p_k = a_k * p_{k-1} + p_{k-2} */
        p = (seq_t)a_term * p_prev + p_prevprev;

        /* shift for next iteration */
        p_prevprev = p_prev;
        p_prev = p;

        if (i == n) break;        /* finished desired term */

        /* compute next continued‑fraction term */
        m = d * a_term - m;
        d = (N - m * m) / d;
        a_term = (a0 + m) / d;
    }

    return p;
}
