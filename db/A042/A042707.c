/*
 * A042707  Denominators of continued fraction convergents to sqrt(883).
 * https://oeis.org/A042707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 21, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042707: Denominators of continued fraction convergents to sqrt(883) */
/* Recurrence:  q_{-2}=1,  q_{-1}=0,  q_n = a_n*q_{n-1} + q_{n-2} */
/* with a_0 = floor(sqrt(883)) = 29 and for k≥0 the partial quotients a_{k+1} */
/* are produced by the standard continued‑fraction algorithm for sqrt(883). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants for sqrt(883) */
    const uint64_t S   = 883ULL;
    const uint64_t a0  = 29ULL;          /* floor(sqrt(883)) */

    /* Initial continued‑fraction state */
    uint64_t m = 0;                      /* m_0 */
    uint64_t d = 1;                      /* d_0 */
    seq_t   a_k = (seq_t)a0;             /* current partial quotient */

    /* Recurrence initial values */
    seq_t q_prevprev = (seq_t)1; /* q_{-2} */
    seq_t q_prev     = (seq_t)0; /* q_{-1} */
    seq_t q_cur      = 0;        /* placeholder */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Compute q_k = a_k * q_{k-1} + q_{k-2} */
        q_cur = (seq_t)a_k * q_prev + q_prevprev;

        /* Shift for next step */
        q_prevprev = q_prev;
        q_prev     = q_cur;

        /* If we've reached the desired index, exit */
        if (k == n)
            break;

        /* Generate next partial quotient a_{k+1} */
        m = d * (uint64_t)a_k - m;
        /* Use 128‑bit arithmetic to avoid overflow when squaring m */
        d = (uint64_t)((S - (uint64_t)((__int128)m * m)) / d);
        a_k = (seq_t)((a0 + m) / d);
    }

    return q_cur;   /* q_n */
}
