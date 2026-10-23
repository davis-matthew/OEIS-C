/*
 * A011935  a(n) = floor( n*(n-1)*(n-2)*(n-3)/25 ).
 * https://oeis.org/A011935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(n-1)*(n-2)*(n-3) / 25 )
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 292, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4U)
        return (seq_t)0;
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn - 1);
    prod = prod * (nn - 2);
    prod = prod * (nn - 3);
    return prod / 25;
}
