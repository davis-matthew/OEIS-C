/*
 * A042260  Numerators of continued fraction convergents to sqrt(656).
 * https://oeis.org/A042260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent to sqrt(656)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 24, u64 n <= 49, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t D = (seq_t)656;
    const seq_t a0 = (seq_t)25;
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_cur = a0;
    seq_t p_prevprev = 0;
    seq_t p_prev = 1;
    seq_t p_cur = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        p_cur = a_cur * p_prev + p_prevprev;
        if (k == n)
            return p_cur;
        p_prevprev = p_prev;
        p_prev = p_cur;
        m = d * a_cur - m;
        d = (D - m * m) / d;
        a_cur = (a0 + m) / d;
    }
    return p_cur;   /* unreachable */
}
