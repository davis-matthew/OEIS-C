/*
 * A042631  Denominators of continued fraction convergents to sqrt(845).
 * https://oeis.org/A042631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A042631 – Denominators of continued fraction convergents to sqrt(845)
   Calculation via standard recurrence with period 5:
   sqrt(845) = [29; 14,1,1,14,58] repeating.
   Denominator q_n satisfies:
      q_{-2}=1, q_{-1}=0,
      q_k = a_k * q_{k-1} + q_{k-2}
   where a_0=29 and for k≥1, a_k follows the 5‑periodic sequence
   (14,1,1,14,58).  The implementation produces q_n directly. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* constants for sqrt(845) */
    const uint64_t N   = 845ULL;     /* N = 845 */
    const uint64_t a0  = 29ULL;      /* floor(sqrt(N)) */

    /* continued fraction state */
    uint64_t m   = 0;                /* current m_k      */
    uint64_t d   = 1;                /* current d_k      */
    uint64_t a_k = a0;               /* current a_k      */

    /* convergent denominators */
    seq_t q_m2 = 1;                  /* q_{k-2} */
    seq_t q_m1 = 0;                  /* q_{k-1} */
    seq_t q_k  = 0;                  /* q_k for current k */

    /* iterate up to n : k = 0 ... n */
    for (idx_t k = 0; k <= n; ++k) {
        /* compute q_k = a_k * q_{k-1} + q_{k-2} */
        q_k = (seq_t)a_k * q_m1 + q_m2;

        /* if sufficient, return */
        if (k == n) return q_k;

        /* advance continued fraction digits */
        uint64_t m_next = d * a_k - m;
        uint64_t d_next = (N - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* rotate q indices */
        q_m2 = q_m1;
        q_m1 = q_k;

        /* update state for next iteration */
        m   = m_next;
        d   = d_next;
        a_k = a_next;
    }

    /* unreachable but satisfy compiler */
    return 0;
}
