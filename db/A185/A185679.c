/*
 * A185679  Number of digits in decimal expansion of n^2.
 * https://oeis.org/A185679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1 + floor(log_10(n^2)) for n > 0, a(0) = 1
 * Range:     u8 n <= 15, u16 n <= 1214, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t sq = (seq_t)n * (seq_t)n;   /* n^2 */
    seq_t ten = 10;
    seq_t digits = 1;
    while (sq >= ten) {
        sq /= ten;
        ++digits;
    }
    return digits;
}
