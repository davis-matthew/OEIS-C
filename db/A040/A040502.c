/*
 * A040502  Continued fraction for sqrt(525).
 * https://oeis.org/A040502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040502: Continued fraction terms for sqrt(525) */
seq_t a(idx_t n)
{
    const seq_t D  = (seq_t)525;
    const seq_t a0 = (seq_t)22;
    if (n == 0) return a0;

    seq_t m = 0;
    seq_t d = 1;
    seq_t a_cur = a0;

    for (idx_t i = 1; i <= n; ++i) {
        m = d * a_cur - m;
        d = (D - m * m) / d;
        a_cur = (a0 + m) / d;
    }
    return a_cur;
}
