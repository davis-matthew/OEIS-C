/*
 * A158665  a(n) = 841*n^2 + 29.
 * https://oeis.org/A158665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 841*n^2 + 29
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 2259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return (seq_t)841 * sn * sn + (seq_t)29;
}
