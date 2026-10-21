/*
 * A055832  T(n,n-5), where T is the array in A055830.
 * https://oeis.org/A055832
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-3)*(n-4)*(n^2+13*n+6)/24
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 296, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) return (seq_t)0;   /* below the offset – value irrelevant */
    seq_t N = (seq_t)n;
    seq_t prod = (N-3) * (N-4) * (N*N + 13*N + 6);
    return prod / (seq_t)24;
}
