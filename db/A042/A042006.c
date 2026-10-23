/*
 * A042006  Numerators of continued fraction convergents to sqrt(526).
 * https://oeis.org/A042006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numerators of continued fraction convergents to sqrt(526).
    a(0) = 22, a(1) = 23, a(n) = a_{n-1}*a_coeff[n] + a_{n-2},
    where a_coeff[n] are the continued‑fraction coefficients of sqrt(526). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants for D = 526 */
    const idx_t D   = 526;      /* Non‑square integer */
    const idx_t a0  = 22;       /* floor(sqrt(526)) */

    if (n == 0)                /* first convergent a0/1 */
        return (seq_t)a0;

    /* Recurrence variables:
       p_{-1} = 1,   p_0 = a0
       m_0 = 0, d_0 = 1, a_0 = a0
     */
    seq_t p_prev_prev = 1;          /* p_{-1} */
    seq_t p_prev      = (seq_t)a0;  /* p_0 */

    idx_t m = 0;
    idx_t d = 1;
    idx_t a_coeff = a0;             /* equal to a_0 initially */

    /* Iterate up to the requested index */
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute next triple (m, d, a_coeff) for continued fraction */
        m = d * a_coeff - m;
        d = (D - m * m) / d;
        a_coeff = (a0 + m) / d;

        /* Recurrence for convergent numerators: p_n = a_coeff * p_{n-1} + p_{n-2} */
        seq_t p_next = a_coeff * p_prev + p_prev_prev;

        p_prev_prev = p_prev;
        p_prev = p_next;
    }

    return p_prev;   /* p_n = a(n) */
}
