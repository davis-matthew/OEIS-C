/*
 * A056864  Nearest integer to n^2/10.
 * https://oeis.org/A056864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n^2 / 10) = floor((n^2 + 5) / 10)
 * Range:     u8 n <= 15, u16 n <= 316, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;      /* n^2 in seq_t */
    return (sq + (seq_t)5) / (seq_t)10;  /* round to nearest integer */
}
