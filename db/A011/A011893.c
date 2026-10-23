/*
 * A011893  a(n) = floor( n*(n-1)*(n-2)/11 ).
 * https://oeis.org/A011893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(n-1)*(n-2)/11 )
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1709, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3U) return 0;
    seq_t prod = (seq_t)n * (n-1) * (n-2);
    return prod / 11;
}
