/*
 * A154295  a(n) = 81*n^2 - 90*n + 26.
 * https://oeis.org/A154295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 81*n^2 - 90*n + 26
 * Range:     u8 n <= 2, u16 n <= 28, u32 n <= 7282, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * nn * (seq_t)81 - nn * (seq_t)90 + (seq_t)26;
}
