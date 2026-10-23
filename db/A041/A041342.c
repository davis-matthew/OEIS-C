/*
 * A041342  Numerators of continued fraction convergents to sqrt(185).
 * https://oeis.org/A041342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=13. For n>=1, a(n) is the numerator of the n‑th convergent
 *            to sqrt(185). The continued fraction coefficients are: c[0] =
 *            13, c[1..] = cycle {1, 1, 1, 1, 26} repeating. The numerators
 *            satisfy p[-2]=0, p[-1]=1 and p(k) = c(k)*p(k-1)+p(k-2)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction coefficients after the first term. */
    static const int period[5] = {1, 1, 1, 1, 26};
    const int period_len = 5;

    /* p_{-2} = 0, p_{-1} = 1 (use long loop with indices from 0). */
    seq_t p_prevprev = 0; /* p(k-2) */
    seq_t p_prev    = 1;  /* p(k-1) */
    seq_t p_curr    = 0;  /* placeholder for p(k) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t coeff;
        if (k == 0) {
            coeff = (seq_t)13;          /* c0 */
        } else {
            /* k-1 is at least 0, so the wrap-around is safe. */
            int idx = (int)((k - 1) % period_len);
            coeff  = (seq_t)period[idx];
        }
        /* p(k) = coeff * p(k-1) + p(k-2) */
        p_curr = coeff * p_prev + p_prevprev;

        /* Shift indices for next iteration */
        p_prevprev = p_prev;
        p_prev    = p_curr;
    }

    return p_curr;    /* p(n) now holds the desired value */
}
