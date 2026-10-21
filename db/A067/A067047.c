/*
 * A067047  a(n) = lcm(n, n+1, n+2, n+3)/12.
 * https://oeis.org/A067047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)/24 if 3∤n a(n) =
 *            n*(n+1)*(n+2)*(n+3)/72 if 3|n (i.e. a(n) =
 *            lcm(n,n+1,n+2,n+3)/12)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 397, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t prod = nn * (nn + 1) * (nn + 2) * (nn + 3);
    /* divisor depends on n mod 3 */
    seq_t div = (n % 3 == 0) ? (seq_t)72 : (seq_t)24;
    return prod / div;
}
