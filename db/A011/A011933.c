/*
 * A011933  a(n) = floor( n*(n-1)*(n-2)*(n-3)/23 ).
 * https://oeis.org/A011933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( n*(n-1)*(n-2)*(n-3) / 23 )
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 273, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    seq_t nn = (seq_t)n;
    seq_t prod = (nn * (nn - 1)) * (nn - 2) * (nn - 3);
    return prod / 23;
}
