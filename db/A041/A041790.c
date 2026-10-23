/*
 * A041790  Numerators of continued fraction convergents to sqrt(416).
 * https://oeis.org/A041790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) is the nth numerator of the convergents to sqrt(416). Let
 *            a₀ = 20 and for n ≥ 1 let the coefficients aₙ repeat the period
 *            〈2, 1, 1, 9, 1, 1, 2, 40〉. The numerators satisfy the recurrence
 *            p_{-2} = 0, p_{-1} = 1, p_n = a_n · p_{n-1} + p_{n-2}, and a(n)
 *            = p_n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  Periodic continued‑fraction coefficients for sqrt(416). */
    static const idx_t period[8] = {2, 1, 1, 9, 1, 1, 2, 40};

    seq_t p_nm2 = 0;          /* p_{-2} */
    seq_t p_nm1 = 1;          /* p_{-1} */
    seq_t p_curr = 0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai;
        if (i == 0)
            ai = 20;                                     /* a₀ */
        else
            ai = period[(i - 1) % 8];                    /* a_n for n ≥ 1 */

        p_curr = ai * p_nm1 + p_nm2;   /* recurrence */
        p_nm2  = p_nm1;
        p_nm1  = p_curr;
    }
    return p_curr;                     /* a(n) = p_n */
}
