/*
 * A062938  a(n) = n*(n+1)*(n+2)*(n+3)+1 = (n^2 + 3*n + 1)^2.
 * https://oeis.org/A062938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 3*n + 1)^2
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 254, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t = nn * nn + (seq_t)3 * nn + 1;
    return t * t;
}
