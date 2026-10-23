/*
 * A041846  Numerators of continued fraction convergents to sqrt(445).
 * https://oeis.org/A041846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(445)
 *
 * The recurrence follows the standard construction of continued fraction
 * convergents for a quadratic irrational.
 * Let D = 445, a0 = ⌊√D⌋ = 21.  Initialise
 *   m0 = 0, d0 = 1, a0 = 21.
 * For k ≥ 0:
 *   mk+1 = dk * ak – mk
 *   dk+1 = (D – mk+1²) / dk
 *   ak+1 = ⌊ (a0 + mk+1) / dk+1 ⌋
 *
 * Convergents’ numerators pk satisfy
 *   p−2 = 0, p−1 = 1,
 *   pk   = ak · pk−1 + pk−2
 *
 * a(n) equals pk for n = 0, 1, 2, …  (a(0) = 21).
 */

#include <stdint.h>

#define SQRT445 21ULL
#define D 445ULL

seq_t a(idx_t n)
{
    idx_t m = 0, d = 1;
    idx_t a_cur = SQRT445;          /* a0 */
    seq_t p_prev2 = 0;              /* p_{-1} */
    seq_t p_prev1 = 1;              /* p_{-2} */
    seq_t p_curr;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_seq = (seq_t)a_cur;
        p_curr = a_seq * p_prev1 + p_prev2;   /* pk = ak * pk-1 + pk-2 */
        if (i == n) return p_curr;           /* answer */

        /* Prepare next triple (m,d,a) */
        idx_t m_next = d * a_cur - m;
        idx_t d_next = (D - m_next * m_next) / d;
        a_cur = (SQRT445 + m_next) / d_next;

        /* Shift numerators for next iteration */
        m = m_next;
        d = d_next;
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }
    /* Should never reach here */
    return (seq_t)0;
}
