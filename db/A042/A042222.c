/*
 * A042222  Numerators of continued fraction convergents to sqrt(637).
 * https://oeis.org/A042222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of the continued‑fraction convergents to sqrt(637).
   Recurrence:
     p_{-2} = 0,  p_{-1} = 1
     p_k    = a_k * p_{k-1} + p_{k-2},  k ≥ 0
   where the partial quotients a_k are generated from the continued
   fraction of sqrt(637).  The sequence a(n) is defined by
     a(n) = p_n    (with p_0 = 25).                                 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t D = 637;           /* radicand */

    /* integer part of sqrt(637) */
    uint64_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D)
        ++a0;

    /* recurrence initial values */
    seq_t p_nm2 = 0;                  /* p_{-2} */
    seq_t p_nm1 = 1;                  /* p_{-1} */
    seq_t p_cur;

    /* continued‑fraction variables */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a_k = a0;                /* a_0 */

    for (idx_t i = 0; i <= n; ++i) {
        /* p_i = a_i * p_{i-1} + p_{i-2} */
        p_cur = (seq_t)a_k * p_nm1 + p_nm2;
        if (i == n)
            return p_cur;

        /* generate next partial quotient a_{i+1} */
        uint64_t m_next = d * a_k - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* shift recurrence variables */
        m = m_next;
        d = d_next;
        a_k = a_next;

        p_nm2 = p_nm1;
        p_nm1 = p_cur;
    }

    /* should not reach here */
    return 0;
}
