/*
 * A041694  Numerators of continued fraction convergents to sqrt(367).
 * https://oeis.org/A041694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numerators of continued fraction convergents to sqrt(367).
   Computed by the standard recurrence
        p_(-1) = 1, p_0 = ⌊√367⌋ = 19
        p_k   = a_k * p_(k-1) + p_(k-2),   k ≥ 1
   where a_k are the partial quotients of the continued fraction
   expansion of √367.  The partial quotients are generated on
   the fly using the classic algorithm for quadratic irrationals. */

#include <stdint.h>   /* for uint64_t, used as idx_t when needed */

seq_t a(idx_t n)
{
    /* constants for the quadratic irrational √367 */
    const idx_t sqrtD   = 19;   /* floor(√367) */
    const idx_t D       = 367;

    /* Handle the first term directly */
    if (n == 0)
        return (seq_t)sqrtD;

    /* Continued‑fraction state variables */
    idx_t m = 0;          /* m_0 */
    idx_t d = 1;          /* d_0 */
    idx_t a_coeff = sqrtD;/* a_0 */

    /* Convergent numerators */
    seq_t p_prevprev = (seq_t)1;          /* p_(-1) */
    seq_t p_prev     = (seq_t)sqrtD;     /* p_0  */
    seq_t p_curr     = 0;

    /* Iterate until we reach the desired index */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Generate the next partial quotient a_k */
        idx_t m_new = d * a_coeff - m;
        idx_t d_new = (D - m_new * m_new) / d;
        idx_t a_new = (sqrtD + m_new) / d_new;

        /* Advance the continued‑fraction state */
        a_coeff = a_new;
        m       = m_new;
        d       = d_new;

        /* Compute the new convergent numerator */
        p_curr = (seq_t)a_new * p_prev + p_prevprev;

        /* Shift for the next iteration */
        p_prevprev = p_prev;
        p_prev     = p_curr;
    }

    return p_curr;
}
