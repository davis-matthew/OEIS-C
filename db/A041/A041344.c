/*
 * A041344  Numerators of continued fraction convergents to sqrt(186).
 * https://oeis.org/A041344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(186). */
seq_t a(idx_t n)
{
    const seq_t A0 = (seq_t)13;           /* floor(sqrt(186)) */
    const seq_t S  = (seq_t)186;          /* value under the square root */

    seq_t h_prev2 = 0;   /* h_{-2} */
    seq_t h_prev1 = 1;   /* h_{-1} */

    seq_t m   = 0;       /* current m   */
    seq_t d   = 1;       /* current d   */
    seq_t a_coeff = A0;  /* current a_i */

    for (idx_t i = 0; i <= n; ++i) {
        /* compute numerator for the current convergent */
        seq_t h = a_coeff * h_prev1 + h_prev2;
        if (i == n) return h;                    /* finished */

        /* advance to the next coefficient in the continued fraction */
        seq_t m_next = d * a_coeff - m;
        seq_t d_next = (S - m_next * m_next) / d;
        seq_t a_next = (A0 + m_next) / d_next;

        /* shift state for next iteration */
        h_prev2 = h_prev1;
        h_prev1 = h;
        m = m_next;
        d = d_next;
        a_coeff = a_next;
    }

    /* control never reaches here */
    return 0;
}
