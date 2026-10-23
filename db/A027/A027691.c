/*
 * A027691  a(n) = n^2 + n + 6.
 * https://oeis.org/A027691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + n + 6
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * sn + sn + (seq_t)6;
}
