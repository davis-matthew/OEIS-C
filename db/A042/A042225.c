/*
 * A042225  Denominators of continued fraction convergents to sqrt(638).
 * https://oeis.org/A042225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(638) */
#include <stdint.h>

seq_t a(idx_t n) {
    const uint64_t N  = 638ULL;     /* integer under the square root */
    const uint64_t a0 = 25ULL;      /* floor(sqrt(638)) */

    /* q_{-2} = 1, q_{-1} = 0 for the recurrence:
       q_k = a_k * q_{k-1} + q_{k-2}     */
    seq_t q_prevprev = (seq_t)1;
    seq_t q_prev     = (seq_t)0;

    uint64_t m = 0;          /* m_0 */
    uint64_t d = 1;          /* d_0 */
    uint64_t a_curr = a0;    /* a_0 */

    for (idx_t k = 0; ; ++k) {
        seq_t q_curr = (seq_t)a_curr * q_prev + q_prevprev;
        if (k == n) return q_curr;

        /* Compute next partial quotient a_{k+1} */
        uint64_t m_next = (uint64_t)((unsigned __int128)d * a_curr - m);
        uint64_t d_next = (uint64_t)(((N - (uint64_t)((unsigned __int128)m_next * m_next))) / d);
        uint64_t a_next = (a0 + m_next) / d_next;

        /* Advance for next iteration */
        m = m_next;
        d = d_next;
        a_curr = a_next;
        q_prevprev = q_prev;
        q_prev = q_curr;
    }
}
