/*
 * A041318  Numerators of continued fraction convergents to sqrt(173).
 * https://oeis.org/A041318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Convergent numerator recurrence for sqrt(173).
   a(n) = p_n, where p_{-2}=0, p_{-1}=1, p_k = a_k * p_{k-1} + p_{k-2}
   with a_0 = ⌊√173⌋ = 13, and for k≥1
      a_k = ⌊ (a_0 + m_k) / d_k ⌋
      m_{k+1} = d_k * a_k - m_k
      d_{k+1} = (173 - m_{k+1}^2) / d_k
   The function computes the numerator for any non‑negative index n
   in O(n) time, without allocating dynamic memory. */

#include <stdint.h>   /* for standard integer types (idx_t, seq_t supplied) */

seq_t a(idx_t n)
{
    /* Constants for sqrt(173) */
    const idx_t a0     = 13;      /* floor(sqrt(173)) */
    const idx_t D      = 173;     /* the square‑free part */

    /* State for continued‑fraction generation */
    idx_t   m = 0;                /* m_0 */
    idx_t   d = 1;                /* d_0 */
    idx_t   a_k;                  /* current partial quotient */

    /* Recurrence for numerators: p_{-2}=0, p_{-1}=1 */
    seq_t p_prev_prev = 0;        /* p_{k-2} */
    seq_t p_prev      = 1;        /* p_{k-1} */
    seq_t p_k         = 0;        /* current numerator p_k */

    /* Special case: n == 0 */
    if (n == 0) {
        /* p_0 = a0 * p_{-1} + p_{-2} */
        p_k = (seq_t)a0 * p_prev + p_prev_prev;
        return p_k;
    }

    /* k = 0 */
    a_k = a0;
    p_k = (seq_t)a_k * p_prev + p_prev_prev;
    /* Prepare for next k */
    p_prev_prev = p_prev;
    p_prev      = p_k;
    /* Update m and d for k = 1 */
    {
        idx_t m_next = d * a_k - m;
        idx_t d_next = (D - m_next * m_next) / d;
        m = m_next; d = d_next;
    }

    /* Iterate for k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k) {
        if (k == n) {
            /* a_k already computed for the final step below */
        }
        /* Compute a_k for this k */
        a_k = (a0 + m) / d;

        /* Numerator recurrence */
        p_k = (seq_t)a_k * p_prev + p_prev_prev;

        if (k == n) break;  /* no need to update state after last term */

        /* Update state for next iteration */
        p_prev_prev = p_prev;
        p_prev      = p_k;

        idx_t m_next = d * a_k - m;
        idx_t d_next = (D - m_next * m_next) / d;
        m = m_next; d = d_next;
    }

    return p_k;
}
