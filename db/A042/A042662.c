/*
 * A042662  Numerators of continued fraction convergents to sqrt(861).
 * https://oeis.org/A042662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = Numerator of the nth convergent of the continued fraction
 *            of √861. Recurrence: let h_{-2}=0, h_{-1}=1; then for k≥0 a_k =
 *            continued‑fraction partial quotient of √861 (a_0 = ⌊√861⌋ = 29)
 *            h_k = a_k * h_{k-1} + h_{k-2} a(n) = h_n. The continued‑fraction
 *            coefficients are generated with the sta...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 31, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* floor(√861) */
    const seq_t A0 = 29;
    /* continued‑fraction state: m_k, d_k, a_k */
    uint64_t m = 0;          /* m_0 */
    uint64_t d = 1;          /* d_0 */
    uint64_t a_curr = 29;    /* a_0 */

    /* convergent numerators: h_{-2} = 0, h_{-1} = 1 */
    seq_t h_nm2 = 0;
    seq_t h_nm1 = 1;

    for (idx_t k = 0; k <= n; ++k) {
        /* h_k = a_k * h_{k-1} + h_{k-2} */
        seq_t h_cur = (seq_t)a_curr * h_nm1 + h_nm2;
        if (k == n)         /* return the requested term */
            return h_cur;

        /* advance state for next partial quotient */
        h_nm2 = h_nm1;
        h_nm1 = h_cur;

        uint64_t m_next = d * a_curr - m;
        uint64_t d_next = (861 - m_next * m_next) / d;
        uint64_t a_next = (29 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_curr = a_next;
    }

    /* unreachable but keeps compiler happy */
    return 0;
}
