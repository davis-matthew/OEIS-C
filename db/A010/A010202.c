/*
 * A010202  Continued fraction for sqrt(149).
 * https://oeis.org/A010202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = coefficient of the continued fraction expansion of
 *            sqrt(149)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)12;
    }
    static seq_t period[20];
    static idx_t period_len = 0;
    if (period_len == 0) {
        const seq_t D = 149;
        seq_t a0 = 0;
        while ((a0 + 1) * (a0 + 1) <= D) {
            ++a0;
        }
        seq_t m   = 0;
        seq_t d   = 1;
        seq_t a   = a0;
        do {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[period_len++] = a;
        } while (a != 2 * a0);
    }
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
