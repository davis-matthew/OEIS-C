/*
 * A040336  Continued fraction for sqrt(355).
 * https://oeis.org/A040336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = continued fraction terms of sqrt(355) * with a(0) =
 *            floor(sqrt(355)) = 18 * a(n) for n>0 computed via the standard
 *            recurrence for quadratic irrationals
 * Range:     u8 n <= 1, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t S   = (seq_t)355;   /* the radicand */
    const seq_t a0  = (seq_t)18;    /* floor(sqrt(355)) */

    if (n == 0) return a0;

    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a_term = a0;

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute next m, d, and term */
        m = d * a_term - m;                /* m_{k+1} = d_k * a_k - m_k */
        d = (S - m * m) / d;               /* d_{k+1} = (S - m_{k+1}²) / d_k */
        a_term = (a0 + m) / d;             /* a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1}) */

        if (i == n) return a_term;
    }

    /* The loop always returns before reaching here */
    return a0;
}
