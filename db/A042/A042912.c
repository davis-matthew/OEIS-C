/*
 * A042912  Numerators of continued fraction convergents to sqrt(988).
 * https://oeis.org/A042912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: numerator of the n-th convergent of sqrt(988).
   a(0) = 31, a(1) = 63.
   For n ≥ 2, let c[n] be the continued‑fraction coefficient of √988:
   c[0] = 31, and for n ≥ 1, c[n] repeats the block
   2, 3, 4, 1, 20, 6, 1, 14, 1, 6, 20, 1, 4, 3, 2, 62.
   Then   a(n) = c[n] * a(n–1) + a(n–2). */
seq_t a(idx_t n)
{
    /* Periodic continued fraction coefficients after the initial 31. */
    static const seq_t period[16] = {2, 3, 4, 1, 20, 6, 1, 14,
                                     1, 6, 20, 1, 4, 3, 2, 62};

    /* Base values for the recurrence: p_{-2}=0, p_{-1}=1. */
    seq_t p_prevprev = 0;
    seq_t p_prev     = 1;
    seq_t p          = 0;
    seq_t coef;

    for (idx_t k = 0; k <= n; ++k)
    {
        if (k == 0)
            coef = 31;                       /* c[0] */
        else
            coef = period[(k - 1) % 16];     /* periodic part */

        p = coef * p_prev + p_prevprev;      /* recurrence */
        /* shift for next iteration */
        p_prevprev = p_prev;
        p_prev     = p;
    }
    return p; /* a(n) */
}
