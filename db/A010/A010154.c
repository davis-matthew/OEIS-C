/*
 * A010154  Continued fraction for sqrt(76).
 * https://oeis.org/A010154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 n <= 9945, u16 n <= 9945, u32 n <= 9945, u64 n <= 9945,
 *            u128 n <= 9945, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction expansion for sqrt(76)
   a(0) = floor(sqrt(76)) = 8; subsequent terms are produced by the
   standard periodic algorithm for quadratic surds; the period length is 12. */
seq_t a(idx_t n)
{
    const seq_t a0  = (seq_t)8;          /* floor(sqrt(76)) */
    const seq_t rad = (seq_t)76;         /* the radicand */

    static seq_t period[128];
    static idx_t period_len = 0;

    if (period_len == 0) {
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;
        idx_t i = 0;
        do {
            /* m_{k+1} = d_k * a_k - m_k */
            m = d * a - m;
            /* d_{k+1} = (rad - m_{k+1}^2) / d_k  */
            seq_t mm = m * m;
            d = (seq_t)((rad - mm) / d);
            /* a_{k+1} = floor( (a0 + m_{k+1}) / d_{k+1} ) */
            a = (seq_t)((a0 + m) / d);
            period[i++] = a;
        } while (a != 2 * a0);     /* period ends when a == 2*a0 (16) */
        period_len = i;
    }

    if (n == 0)
        return a0;
    else
        return period[(n - 1) % period_len];
}
