/*
 * A042803  Denominators of continued fraction convergents to sqrt(932).
 * https://oeis.org/A042803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent of sqrt(932) (OEIS
 *            A042803). The sequence is generated from the continued‑fraction
 *            expansion of sqrt(932). The denominators k_n are obtained by the
 *            recurrence k_-2 = 1 , k_-1 = 0, k_n = a_n * k_{n-1} + k_{n-2},
 *            where a_n is the n‑th partial quotient of sq...
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 24, u64 n <= 46, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t D   = 932;   /* the square‑free part under the root */
    const idx_t a0  = 30;    /* floor(sqrt(932)) */

    seq_t k_prevprev = 1;    /* k_{-2} */
    seq_t k_prev     = 0;    /* k_{-1} */

    idx_t m = 0;
    idx_t d = 1;
    idx_t a_cur = a0;

    for (idx_t i = 0; i <= n; ++i)
    {
        /* compute k_i */
        seq_t k_next = (seq_t)a_cur * k_prev + k_prevprev;

        /* shift for next iteration */
        k_prevprev = k_prev;
        k_prev = k_next;

        if (i == n)
            return k_next;

        /* generate next partial quotient a_{i+1} */
        m = (idx_t)(d * a_cur - m);
        d = (idx_t)((D - m * m) / d);
        a_cur = (idx_t)((a0 + m) / d);
    }
    return 0;  /* unreachable, keeps compiler quiet */
}
