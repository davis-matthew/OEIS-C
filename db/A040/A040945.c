/*
 * A040945  Continued fraction for sqrt(977).
 * https://oeis.org/A040945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction coefficients of sqrt(977) computed by
 *            standard algorithm
 * Range:     u8 n <= 1, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t N  = 977;   /* Number whose square root is expanded */
    const seq_t a0 = 31;    /* floor(sqrt(N)) */

    seq_t m  = 0;
    seq_t d  = 1;
    seq_t a_val = a0;

    if (n == 0)
        return a0;

    idx_t i;
    for (i = 1; i <= n; ++i) {
        m = d * a_val - m;
        d = (N - m * m) / d;
        a_val = (a0 + m) / d;
    }
    return a_val;
}
