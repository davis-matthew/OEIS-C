/*
 * A042666  Numerators of continued fraction convergents to sqrt(863).
 * https://oeis.org/A042666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(863) */
/* a(0) = 29, a(1) = 59, a(2) = 88, ...   OEIS A042666 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t N   = 863ULL;
    const uint64_t a0  = 29ULL;          /* floor(sqrt(863))           */
    uint64_t m = 0ULL;                   /* m_k in the continued‑fraction algorithm   */
    uint64_t d = 1ULL;                   /* d_k                                              */
    uint64_t a = a0;                     /* a_k = floor((a0 + m_k)/d_k)                    */
    seq_t P_nm2 = (seq_t)0;              /* P_{-2} = 0                                       */
    seq_t P_nm1 = (seq_t)1;              /* P_{-1} = 1                                       */
    idx_t  k    = 0;                     /* current index                                     */

    while (1) {
        /* compute numerator of the k‑th convergent */
        seq_t P_k = (seq_t)a * P_nm1 + P_nm2;
        if (k == n) return P_k;

        /* shift for next iteration */
        P_nm2 = P_nm1;
        P_nm1 = P_k;

        /* advance the continued‑fraction variables */
        uint64_t m_next = d * a - m;
        uint64_t d_next = (N - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;
        k++;
    }
}
