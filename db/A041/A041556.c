/*
 * A041556  Numerators of continued fraction convergents to sqrt(296).
 * https://oeis.org/A041556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: p_n = a_n * p_{n-1} + p_{n-2}.  The partial quotients of
 * sqrt(296) are: a_0 = 17, a_1.. a_ν (ν ≥ 1) repeat the period
 * [4, 1, 7, 1, 4, 34].  a(0) is the numerator of the 0‑th convergent,
 * a(1) the numerator of the 1‑st convergent, etc. */
seq_t a(idx_t n)
{
    /* coefficients of the continued fraction for sqrt(296) */
    static const seq_t period[6] = {4, 1, 7, 1, 4, 34};

    seq_t p_prev2 = 0;   /* p_{-2} */
    seq_t p_prev1 = 1;   /* p_{-1} */

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t a_i;

        if (i == 0)
            a_i = 17;                 /* a_0 */
        else
            a_i = period[(i - 1) % 6]; /* periodic part starts at a_1 */

        seq_t p = a_i * p_prev1 + p_prev2;   /* recurrence */

        p_prev2 = p_prev1;
        p_prev1 = p;
    }

    return p_prev1;
}
