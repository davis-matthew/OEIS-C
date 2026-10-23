/*
 * A158729  a(n) = 1156*n^2 - 34.
 * https://oeis.org/A158729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1156*n^2 - 34
 * Range:     u8 none, u16 n <= 7, u32 n <= 1927, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * sn * (seq_t)1156 - (seq_t)34;
}
