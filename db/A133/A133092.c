/*
 * A133092  Row sums of triangle A133091.
 * https://oeis.org/A133092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 for n even, a(n) = n*(n+1)/2 for n odd
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if ((n & 1) == 0)        /* even n */
        return N * N;        /* n*n */
    else                      /* odd n */
        return (N * (N + (seq_t)1)) / (seq_t)2;
}
