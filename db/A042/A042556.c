/*
 * A042556  Numerators of continued fraction convergents to sqrt(807).
 * https://oeis.org/A042556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   Numerators of convergents to sqrt(807)
 * Range:     u8 none, u16 n <= 6, u32 n <= 14, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = floor(sqrt(807)) = 28
   a(n) = a_{n-1} * a_{n-1} + a_{n-2}  where a_{k} are the continued‑fraction coefficients of sqrt(807) */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Constant D = 807 */
    const idx_t D = 807;
    /* Compute a0 = floor(sqrt(D)) using integer arithmetic */
    seq_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= (seq_t)D)
        a0++;
    /* a(0) is the first convergent numerator */
    if (n == 0)
        return a0;

    /* Continued fraction coefficients a_k for sqrt(D) */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_k = a0;          /* a0 */

    /* Convergents p_{-1} = 1, p_0 = a0 */
    seq_t p_prev2 = 1;       /* p_{-1} */
    seq_t p_prev1 = a0;      /* p_0 */
    seq_t p = 0;             /* current numerator */

    for (idx_t k = 1; k <= n; ++k) {
        /* Compute next coefficient a_{k} */
        seq_t m_next = d * a_k - m;
        seq_t d_next = ((seq_t)D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* Update convergent numerator */
        p = a_next * p_prev1 + p_prev2;

        /* Shift indices for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p;
        m = m_next;
        d = d_next;
        a_k = a_next;
    }
    return p;
}
