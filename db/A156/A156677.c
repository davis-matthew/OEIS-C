/*
 * A156677  a(n) = 81*n^2 - 118*n + 43.
 * https://oeis.org/A156677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 81*n^2 - 118*n + 43
 * Range:     u8 n <= 2, u16 n <= 29, u32 n <= 7282, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return ((seq_t)81) * sn * sn - ((seq_t)118) * sn + ((seq_t)43);
}
