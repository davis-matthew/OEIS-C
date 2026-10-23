/*
 * A166148  a(n) = (9*n^2 + 9*n - 16)/2.
 * https://oeis.org/A166148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^2 + 9*n - 16)/2
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 1 */
    seq_t sn = (seq_t)n;
    seq_t val = ((seq_t)9 * sn * sn + (seq_t)9 * sn - (seq_t)16) / 2;
    return val;
}
