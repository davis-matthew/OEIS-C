/*
 * A010897  Minimal scope of a (4,n) difference triangle.
 * https://oeis.org/A010897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = n*(n+1)*(n+2)/6 + n*(n+1)/2 + 3*n - 1
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t term1 = N * (N + 1) * (N + 2) / 6;
    seq_t term2 = N * (N + 1) / 2;
    seq_t term3 = 3 * N;
    return term1 + term2 + term3 - 1;
}
