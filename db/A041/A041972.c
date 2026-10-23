/*
 * A041972  Numerators of continued fraction convergents to sqrt(509).
 * https://oeis.org/A041972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = numerator of the n-th convergent to sqrt(509) */

seq_t a(idx_t n)
{
    /* constants for sqrt(509) */
    const idx_t a0 = 22;          /* floor(sqrt(509)) */

    /* continued‑fraction state */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a_cur = a0;

    /* previous convergents' numerators (p_{-2} and p_{-1}) */
    seq_t p_prev2 = 0;   /* p_{-2} */
    seq_t p_prev1 = 1;   /* p_{-1} */
    seq_t p_cur = 0;     /* current numerator */

    for (idx_t k = 0; k <= n; ++k) {
        if (k == 0) {
            /* p_0 = a0 * p_{-1} + p_{-2} */
            p_cur = (seq_t)a_cur * p_prev1 + p_prev2;
        } else {
            /* next partial quotient a_k */
            m = d * a_cur - m;
            d = (509 - m * m) / d;
            a_cur = (a0 + m) / d;   /* guaranteed integer division */

            /* p_k = a_k * p_{k-1} + p_{k-2} */
            p_cur = (seq_t)a_cur * p_prev1 + p_prev2;
        }

        /* shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_cur;
    }

    return p_cur;
}
