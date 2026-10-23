/*
 * A011862  a(n) = floor(n*(n-1)/9).
 * https://oeis.org/A011862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)/9)
 * Range:     u8 n <= 16, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t prod = nn * nn - nn;   /* n*(n-1) computed without underflow */
    return prod / 9;
}
