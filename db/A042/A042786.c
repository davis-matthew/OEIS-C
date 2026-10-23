/*
 * A042786  Numerators of continued fraction convergents to sqrt(924).
 * https://oeis.org/A042786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of convergents to sqrt(924).
   Recurrence: let a[0] = ⌊√924⌋ = 30.
   For k ≥ 0, a[k+1] is obtained from the continued‑fraction
   algorithm for √924:
       m_{k+1} = d_k * a_k – m_k
       d_{k+1} = (924 – m_{k+1}²) / d_k
       a_{k+1} = ⌊(a_0 + m_{k+1}) / d_{k+1}⌋
   The numerators satisfy
       p_{-1}=1, p_{-2}=0,
       p_k = a_k · p_{k-1} + p_{k-2}   (k ≥ 0)
   a(k) = p_k.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t D  = 924;
    const uint64_t a0 = 30;          /* ⌊√D⌋ */

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t ak = a0;                /* current partial quotient a_k */

    seq_t p_prevprev = 0;            /* p_{k-2} */
    seq_t p_prev    = 1;             /* p_{k-1} */
    seq_t p_cur;

    for (idx_t k = 0; k <= n; ++k) {
        p_cur = (seq_t)ak * p_prev + p_prevprev;
        if (k == n)
            return p_cur;

        /* compute a_{k+1} */
        uint64_t m_next = d * ak - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t ak_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        ak = ak_next;

        p_prevprev = p_prev;
        p_prev = p_cur;
    }

    return 0; /* unreachable */
}
