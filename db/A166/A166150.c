/*
 * A166150  a(n) = 5*n^2 + 5*n - 9.
 * https://oeis.org/A166150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^2 + 5*n - 9
 * Range:     u8 n <= 6, u16 n <= 113, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return (seq_t)5 * sn * (sn + (seq_t)1) - (seq_t)9;
}
