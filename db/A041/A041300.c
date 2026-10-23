/*
 * A041300  Numerators of continued fraction convergents to sqrt(163).
 * https://oeis.org/A041300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, cofr, easy
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(163) continued
 *            fraction
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t D   = 163;   /* D in sqrt(D) */
    const seq_t a0  = 12;    /* floor(sqrt(163)) */

    /* Convergents recurrence: p[-2]=0, p[-1]=1 */
    seq_t p_minus2 = 0;
    seq_t p_minus1 = 1;
    seq_t p_curr   = 0;

    /* State for continued‑fraction coefficients */
    seq_t m   = 0;
    seq_t d   = 1;
    seq_t acf = a0;          /* current coefficient a_k */

    for (idx_t k = 0; k <= n; ++k) {
        /* numerator of the current convergent */
        p_curr = acf * p_minus1 + p_minus2;
        if (k == n) {
            return p_curr;
        }

        /* Compute the next coefficient via standard algorithm */
        seq_t m_new = d * acf - m;
        seq_t d_new = (D - m_new * m_new) / d;
        seq_t a_new = (a0 + m_new) / d_new;

        /* Advance state */
        m = m_new;
        d = d_new;
        acf = a_new;

        /* Shift predecessor numerators */
        p_minus2 = p_minus1;
        p_minus1 = p_curr;
    }

    /* Unreachable, but required for a complete function */
    return 0;
}
