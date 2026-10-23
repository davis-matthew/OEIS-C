/*
 * A156639  a(n) = 169*n^2 - 140*n + 29.
 * https://oeis.org/A156639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 169*n^2 - 140*n + 29
 * Range:     u8 n <= 1, u16 n <= 20, u32 n <= 5041, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return 169 * sn * sn - 140 * sn + 29;
}
