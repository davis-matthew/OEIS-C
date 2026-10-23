/*
 * A161587  a(n) = 13*n^2 + 10*n + 1.
 * https://oeis.org/A161587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*n^2 + 10*n + 1
 * Range:     u8 n <= 4, u16 n <= 70, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return (sn * sn * 13) + (sn * 10) + 1;
}
