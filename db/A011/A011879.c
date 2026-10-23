/*
 * A011879  a(n) = floor( n(n-1)/26 ).
 * https://oeis.org/A011879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)/26)
 * Range:     u8 n <= 82, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    seq_t nn = (seq_t)n;
    return (nn * (nn - 1)) / 26;
}
