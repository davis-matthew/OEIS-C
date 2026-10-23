/*
 * A026060  a(n) = dot_product(n,n-1,...2,1)*(5,6,...,n,1,2,3,4).
 * https://oeis.org/A026060
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 + 15*n - 46)/6
 * Range:     u8 n <= 9, u16 n <= 68, u32 n <= 1623, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) return 0;
    seq_t nn = (seq_t)n;
    seq_t temp = nn * nn + (seq_t)15 * nn - (seq_t)46;
    return (nn * temp) / (seq_t)6;
}
