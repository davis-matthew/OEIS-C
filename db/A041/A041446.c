/*
 * A041446  Numerators of continued fraction convergents to sqrt(239).
 * https://oeis.org/A041446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 30, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(239)
 * Recurrence:
 *   a(0) = 15
 *   for n >= 1
 *     let c_n be the nth continued‑fraction coefficient of sqrt(239)
 *     a(n) = c_n * a(n‑1) + a(n‑2)
 *   (c_1, c_2, …) is periodic: 15, 2, 2, 4, 1, 14, 1, 4, 2, 2, 30, …
 * This implementation computes a(n) directly using the continued‑fraction
 * iteration algorithm.  No table of terms is hard‑coded.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* sqrt(239) constants */
    static const uint64_t S   = 239ULL;   /* number under the square root   */
    static const uint64_t a0  = 15ULL;    /* integer part of sqrt(239)      */

    /* Convergent numerators p_{-2}=0, p_{-1}=1 */
    seq_t p_prev2 = 0;   /* p_{k-2}     */
    seq_t p_prev1 = 1;   /* p_{k-1}     */

    /* Continued‑fraction state */
    uint64_t m = 0;      /* part of the transformation */
    uint64_t d = 1;      /* denominator step            */
    uint64_t a_cur = a0; /* current CF coefficient a_k  */

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute the numerator for step k */
        seq_t p_cur = ((seq_t)a_cur) * p_prev1 + p_prev2;

        /* If this is the desired index, return the result */
        if (k == n) return p_cur;

        /* Advance to the next continued fraction coefficient */
        uint64_t m_next = d * a_cur - m;
        uint64_t d_next = (S - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* Update iteration state */
        m = m_next;
        d = d_next;
        a_cur = a_next;

        /* Shift numerators for the recurrence */
        p_prev2 = p_prev1;
        p_prev1 = p_cur;
    }

    /* Should never reach here */
    return 0;
}
