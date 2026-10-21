/*
 * A082040  a(n) = 9*n^2 + 3*n + 1.
 * https://oeis.org/A082040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^2 + 3*n + 1
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * (9 * sn + 3) + 1;
}
