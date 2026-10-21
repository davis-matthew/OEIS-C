/*
 * A056829  Nearest integer to n^2/6.
 * https://oeis.org/A056829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 3) / 6) // nearest integer to n^2/6
 * Range:     u8 n <= 15, u16 n <= 305, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;
    return (sq + 3) / 6;
}
