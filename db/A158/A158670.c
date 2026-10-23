/*
 * A158670  a(n) = 60*n^2 - 1.
 * https://oeis.org/A158670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 60*n^2 - 1
 * Range:     u8 n <= 2, u16 n <= 33, u32 n <= 8460, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * sn * 60 - 1;
}
