/*
 * A158551  a(n) = 26*n^2 - 1.
 * https://oeis.org/A158551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*n^2 - 1
 * Range:     u8 n <= 3, u16 n <= 50, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return (seq_t)26 * sn * sn - (seq_t)1;
}
