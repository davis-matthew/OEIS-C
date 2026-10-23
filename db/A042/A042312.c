/*
 * A042312  Numerators of continued fraction convergents to sqrt(683).
 * https://oeis.org/A042312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of sqrt(683) Recurrence:
 *            let p_{-2}=0, p_{-1}=1. For k ≥ 0, p_k = a_k · p_{k-1} + p_{k-2}
 *            where a_0=26 and subsequent partial quotients a_k are generated
 *            by the continued‑fraction algorithm for sqrt(683)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t a0 = 26ULL;          /* floor(sqrt(683)) */
    seq_t p_minus2 = 0;
    seq_t p_minus1 = (seq_t)1;
    seq_t p = 0;

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a_val = a0;

    for (idx_t k = 0; k <= n; ++k) {
        /* compute p_k */
        p = (seq_t)a_val * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = p;

        if (k == n) break;            /* finished */

        /* generate next partial quotient */
        uint64_t m_next = d * a_val - m;
        uint64_t d_next = (683ULL - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_val = a_next;
    }
    return p;
}
