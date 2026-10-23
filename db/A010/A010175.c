/*
 * A010175  Continued fraction for sqrt(109).
 * https://oeis.org/A010175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 999, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms of sqrt(109) */
seq_t a(idx_t n)
{
    /* a0 = floor(sqrt(109)) = 10 */
    const seq_t a0 = 10;
    if (n == 0)
        return a0;

    const seq_t D = 109;
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;

    for (idx_t i = 1; i <= n; ++i) {
        /* m = d*a - m */
        m = d * a - m;

        /* d = (D - m*m) / d */
        seq_t m2 = m * m;
        d = (D - m2) / d;

        /* a = (a0 + m) / d */
        a = (a0 + m) / d;
    }
    return a;
}
