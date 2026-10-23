/*
 * A042821  Denominators of continued fraction convergents to sqrt(941).
 * https://oeis.org/A042821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent to sqrt(941)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via the standard recurrence for continued fractions of sqrt(D). */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 */
    if (n == 0) return (seq_t)1;

    /* Constants for D = 941 and a0 = floor(sqrt(941)) */
    const idx_t D = 941;
    const idx_t a0_floor = 30;          /* floor(sqrt(941)) */

    /* Continued fraction variables */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a_k = a0_floor;

    /* Convergent recurrence: p_{-2}=0, p_{-1}=1; q_{-2}=1, q_{-1}=0 */
    seq_t p_prev2 = (seq_t)0, p_prev1 = (seq_t)1;
    seq_t q_prev2 = (seq_t)1, q_prev1 = (seq_t)0;
    seq_t p_k, q_k;

    /* k = 0 (first convergent) */
    p_k = a_k * p_prev1 + p_prev2;     /* 30*1 + 0 = 30 */
    q_k = a_k * q_prev1 + q_prev2;     /* 30*0 + 1 = 1 */
    if (n == 0) return q_k;

    /* shift for the next round */
    p_prev2 = p_prev1;  p_prev1 = p_k;
    q_prev2 = q_prev1;  q_prev1 = q_k;

    /* Iterate from k = 1 up to n */
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute next partial quotient a_{k+1} */
        idx_t m_next = d * a_k - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0_floor + m_next) / d_next;   /* floor division */

        /* Update convergents */
        p_k = a_next * p_prev1 + p_prev2;
        q_k = a_next * q_prev1 + q_prev2;

        if (k == n) return q_k;   /* desired term reached */

        /* Prepare for next iteration */
        p_prev2 = p_prev1;  p_prev1 = p_k;
        q_prev2 = q_prev1;  q_prev1 = q_k;
        m = m_next;  d = d_next;  a_k = a_next;
    }

    /* Should never be reached */
    return (seq_t)0;
}
