/*
 * A168236  a(n) = (6*n - 3*(-1)^n - 1)/4.
 * https://oeis.org/A168236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (6*n - 3*(-1)^n - 1) / 4
 * Range:     u8 n <= 112, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 */
    if ((n & 1) == 0) {
        seq_t tn = (seq_t)n;
        seq_t val = (seq_t)3 * tn;
        return (val >> 1) - 1;
    } else {
        seq_t tn = (seq_t)n;
        seq_t val = (seq_t)3 * tn + 1;
        return val >> 1;
    }
}
