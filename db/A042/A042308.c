/*
 * A042308  Numerators of continued fraction convergents to sqrt(681).
 * https://oeis.org/A042308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent to sqrt(681) in its
 *            continued fraction expansion
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* constants for the square root of 681 */
    const uint64_t D   = 681ULL;   /* D = 681 */
    const uint64_t A0  = 26ULL;    /* floor(sqrt(681)) */

    /* continued‑fraction variables: m, d, and the current partial quotient a_val */
    uint64_t m     = 0ULL;
    uint64_t d     = 1ULL;
    uint64_t a_val = A0;

    /* convergent recurrence: p(-2)=0, p(-1)=1 */
    seq_t p_prev2 = (seq_t)0;
    seq_t p_prev1 = (seq_t)1;
    seq_t p_curr;

    for (idx_t i = 0; i <= n; ++i)
    {
        /* compute p(i) */
        p_curr = (seq_t)a_val * p_prev1 + p_prev2;

        if (i == n)
            return p_curr;

        /* advance to the next partial quotient using the standard algorithm for sqrt(D) */
        uint64_t m_next = d * a_val - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (A0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_val = a_next;

        /* shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    /* must never reach here */
    return (seq_t)0;
}
