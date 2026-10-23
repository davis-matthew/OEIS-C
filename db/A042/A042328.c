/*
 * A042328  Numerators of continued fraction convergents to sqrt(691).
 * https://oeis.org/A042328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 17, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of convergents of continued fraction expansion of sqrt(691) */
seq_t a(idx_t n)
{
    const idx_t D = 691;
    const idx_t a0 = 26;   /* floor(sqrt(D)) */

    idx_t m = 0;
    idx_t d = 1;
    idx_t a_prev = a0;   /* a(i-1) */
    idx_t a_term;

    seq_t p_prev2 = 0;
    seq_t p_prev1 = 1;
    seq_t p_curr = 0;

    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0) {
            a_term = a0;
        } else {
            /* update m and d for the next partial quotient */
            m = d * a_prev - m;
            d = (D - m * m) / d;
            a_term = (a0 + m) / d;
        }

        seq_t a_term_seq = (seq_t)a_term;
        p_curr = a_term_seq * p_prev1 + p_prev2;

        /* shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
        a_prev = a_term;
    }
    return p_curr;
}
