/*
 * A042227  Denominators of continued fraction convergents to sqrt(639).
 * https://oeis.org/A042227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   Denominators of continued fraction convergents to sqrt(639)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Continued‑fraction constants for sqrt(639) */
    const idx_t D   = 639;
    const idx_t a0  = 25;          /* floor(sqrt(639)) */

    /* Variables for the recurrence of the convergent denominators */
    seq_t q_prevprev = 0; /* q_{-1} (used in the recurrence) */
    seq_t q_prev     = 1; /* q_{0}  = 1 */

    /* Variables for generating the continued‑fraction coefficients a_i */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    seq_t q_curr;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Next continued‑fraction coefficient a_i */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        /* q_i = a_i * q_{i-1} + q_{i-2} */
        q_curr = (seq_t)a * q_prev + q_prevprev;

        if (i == n)
            return q_curr;

        /* Shift for next iteration */
        q_prevprev = q_prev;
        q_prev     = q_curr;
    }

    /* Should never reach here */
    return (seq_t)0;
}
