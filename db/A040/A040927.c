/*
 * A040927  Continued fraction for sqrt(958).
 * https://oeis.org/A040927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = term of the continued fraction expansion of sqrt(958). */
/* Uses the standard periodic‐continued‐fraction algorithm. */
seq_t a(idx_t n)
{
    /* Constant: floor(sqrt(958)) = 30 */
    const idx_t D  = 958;
    const idx_t a0 = 30;

    if (n == 0)
        return a0;               /* a(0) = 30 */

    /* States for the continued fraction iteration */
    idx_t m = 0;   /* m0 */
    idx_t d = 1;   /* d0 */
    idx_t a = a0;  /* a0 */

    for (idx_t i = 1; i <= n; ++i) {
        /* m_{i} = d_{i-1} * a_{i-1} - m_{i-1} */
        idx_t m_next = d * a - m;
        /* d_{i} = (D - m_{i}^2) / d_{i-1} */
        idx_t d_next = (D - m_next * m_next) / d;
        /* a_{i} = floor((a0 + m_{i}) / d_{i}) */
        idx_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;
    }

    return (seq_t)a;
}
