/*
 * A011909  a(n) = floor( n*(n-1)*(n-2)/27 ).
 * https://oeis.org/A011909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(n-1)*(n-2)/27 )
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1701, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;
    seq_t N = (seq_t)n;
    seq_t prod = N * (N - 1);
    prod *= (N - 2);
    return prod / (seq_t)27;
}
