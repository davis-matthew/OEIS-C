/*
 * A042258  Numerators of continued fraction convergents to sqrt(655).
 * https://oeis.org/A042258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: p_{-2}=0, p_{-1}=1. For n≥0,
   p_n = a_n * p_{n-1} + p_{n-2},
   where a_n are the continued–fraction partial quotients of sqrt(655)
   (a_0 = 25, a_1 = 1, a_2 = 1, a_3 = 2, a_4 = 5, …).  a(n) is
   the numerator of the n‑th convergent, i.e. p_n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t D    = 655ULL;          /* square‑free part */
    const uint64_t a0   = 25ULL;           /* floor(sqrt(D))              */
    uint64_t m          = 0ULL;            /* m_k                          */
    uint64_t d          = 1ULL;            /* d_k                          */
    uint64_t a_val      = a0;              /* a_k (partial quotient)       */

    seq_t p_nm2 = 0;                       /* p_{-2} */
    seq_t p_nm1 = 1;                       /* p_{-1} */

    /* iterate over partial quotients up to n */
    for (idx_t k = 0; k <= n; ++k) {
        /* compute current convergent numerator */
        seq_t p_k = (seq_t)a_val * p_nm1 + p_nm2;
        if (k == n)
            return p_k;

        /* advance to next partial quotient using standard CF algorithm */
        uint64_t m_next = d * a_val - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        m     = m_next;
        d     = d_next;
        a_val = a_next;

        /* shift previous numerators */
        p_nm2 = p_nm1;
        p_nm1 = p_k;
    }
    /* unreachable; guard against compiler warnings */
    return 0;
}
