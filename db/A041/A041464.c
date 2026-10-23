/*
 * A041464  Numerators of continued fraction convergents to sqrt(248).
 * https://oeis.org/A041464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 15; for n ≥ 1, a(n) = a_{n-1} * a_{n} + a_{n-2}, where
 *            the continued‑fraction partial quotients for sqrt(248) are a0 =
 *            15, a1 = 1, a2 = 2, a3 = 1, a4 = 30 repeating
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p_prev2 = 0; /* p[-2] */
    seq_t p_prev  = 1; /* p[-1] */
    seq_t p_cur   = 0;
    unsigned int a_i;

    static const unsigned int period[4] = {1, 2, 1, 30};

    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0) {
            a_i = 15;
        } else {
            a_i = period[(i - 1) & 3];
        }
        p_cur = (seq_t)a_i * p_prev + p_prev2;
        p_prev2 = p_prev;
        p_prev  = p_cur;
    }
    return p_cur;
}
