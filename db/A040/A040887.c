/*
 * A040887  Continued fraction for sqrt(918).
 * https://oeis.org/A040887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = n-th coefficient of the continued fraction expansion of
 *            √918
 * Range:     u8 n <= 1, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The constant 918 is not a perfect square, so its continued fraction expansion is periodic. */
    const seq_t D   = 918;   /* the radicand */
    const seq_t a0  = 30;    /* floor(√918) */
    seq_t m = 0;            /* initial m */
    seq_t d = 1;            /* initial d */
    seq_t a_coeff = a0;     /* a(0) */

    for (idx_t i = 1; i <= n; ++i) {
        /* Standard recurrence for continued fractions of sqrt(D) */
        m = d * a_coeff - m;
        d = (D - m * m) / d;
        a_coeff = (a0 + m) / d;
    }
    return a_coeff;
}
