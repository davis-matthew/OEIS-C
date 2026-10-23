/*
 * A042309  Denominators of continued fraction convergents to sqrt(681).
 * https://oeis.org/A042309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of convergents to sqrt(681).
   Computed by the standard continued‑fraction algorithm.
   The sequence satisfies
   a(n) = 21486332006830*a(n‑24) – a(n‑48) for n>47,
   but we generate all terms directly from the quadratic irrational. */

#include <stdint.h>

extern seq_t a(idx_t n) {
    /* Constant floor(sqrt(681)) */
    const uint64_t a0 = 26ULL;   /* a0 = ⌊√681⌋ */

    /* Continued fraction state variables */
    uint64_t m = 0;          /* m0 */
    uint64_t d = 1;          /* d0 */
    uint64_t a_cur = a0;     /* a0 */

    /* Convergents denominators: q_-2 = 1, q_-1 = 0 */
    seq_t q_prev2 = 1;       /* Q_{-2} */
    seq_t q_prev1 = 0;       /* Q_{-1} */
    seq_t q_curr;

    for (idx_t k = 0; ; ++k) {
        /* Compute Q_k = a_k * Q_{k-1} + Q_{k-2} */
        q_curr = (seq_t)a_cur * q_prev1 + q_prev2;

        /* Terminate when we have reached the requested index */
        if (k == n) return q_curr;

        /* Prepare for next coefficient: compute m_{k+1}, d_{k+1}, a_{k+1} */
        uint64_t m_next = d * a_cur - m;
        uint64_t d_next = (681ULL - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* Shift state variables for next loop */
        m = m_next;
        d = d_next;
        a_cur = a_next;

        /* Shift denominator previous values */
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }
}
