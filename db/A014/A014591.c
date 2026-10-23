/*
 * A014591  a(n) = floor(n^2/12 + 5/4).
 * https://oeis.org/A014591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 15) / 12)
 * Range:     u8 n <= 15, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn + (seq_t)15;
    return val / (seq_t)12;
}
