/*
 * A040266  Continued fraction for sqrt(283).
 * https://oeis.org/A040266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients of sqrt(283) (OEIS A040266) */
seq_t a(idx_t n)
{
    const seq_t N  = (seq_t)283;
    const seq_t a0 = (seq_t)16;

    if (n == 0) return a0;

    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a_k = a0;

    for (idx_t k = 1; k <= n; ++k) {
        m = d * a_k - m;
        d = (N - m * m) / d;
        a_k = (a0 + m) / d;
    }
    return a_k;
}
