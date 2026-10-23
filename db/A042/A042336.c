/*
 * A042336  Numerators of continued fraction convergents to sqrt(695).
 * https://oeis.org/A042336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  Numerators of continued‑fraction convergents to sqrt(695).
   The recurrence is
     p_{-1} = 1,   p_0 = floor(sqrt(695)) = 26.
   For n ≥ 0:
     m_{n+1} = d_n * a_n - m_n
     d_{n+1} = (695 - m_{n+1}²) / d_n
     a_{n+1} = (26 + m_{n+1}) / d_{n+1}
     p_{n+1} = a_{n+1} * p_n + p_{n-1}
   a(n) is p_n.
   The function recomputes the sequence from scratch for each call,
   so it works for any seq_t type (fixed‑size integer or arbitrary‑size).
*/

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants for sqrt(695) */
    static const idx_t D  = 695;
    static const idx_t a0 = 26;

    /* State initialized for each call (no caching). */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a_cur = a0;

    seq_t p_prev2 = (seq_t)1;          /* p_{-1}   */
    seq_t p_prev  = (seq_t)a0;         /* p_0      */

    if (n == 0) return p_prev;        /* p_0 */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* next CF terms */
        idx_t m_next = d * a_cur - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* next numerator */
        seq_t p_next = (seq_t)a_next * p_prev + p_prev2;

        /* shift for next iteration */
        p_prev2 = p_prev;
        p_prev  = p_next;
        m = m_next;
        d = d_next;
        a_cur = a_next;
    }

    return p_prev;   /* p_n */
}
