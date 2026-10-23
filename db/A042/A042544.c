/*
 * A042544  Numerators of continued fraction convergents to sqrt(801).
 * https://oeis.org/A042544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(801) */
/* Computed by the standard recurrence:
   p_{-1} = 1, p_0 = floor(sqrt(801)), and
   p_n   = a_n * p_{n-1} + p_{n-2},
   where a_n are the terms of the continued‑fraction expansion of sqrt(801). */

seq_t a(idx_t n)
{
    /* Constants for sqrt(801) */
    const idx_t D   = 801;   /* radicand */
    const idx_t a0  = 28;    /* floor(sqrt(801)) */

    /* Handle the offset term a(0) */
    if (n == 0) return (seq_t)a0;

    /* Recurrence initial values */
    seq_t p_prevprev = 1;          /* p_{-1} */
    seq_t p_prev    = (seq_t)a0;   /* p_0 */
    seq_t p_curr = 0;

    /* State for generating the continued‑fraction coefficients a_n (n ≥ 1) */
    idx_t m  = 0,      /* m_0 */
        d  = 1,          /* d_0 */
        ai = a0;        /* a_0 */

    /* Iterate through the convergents up to index n */
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute next coefficient a_i */
        m  = d * ai - m;
        d  = (D - m * m) / d;
        ai = (a0 + m) / d;

        /* Compute p_i */
        p_curr = ai * p_prev + p_prevprev;

        /* Shift for next iteration */
        if (i == n) return p_curr;
        p_prevprev = p_prev;
        p_prev     = p_curr;
    }

    /* The loop always returns; this is a safe return for completeness. */
    return 0;
}
