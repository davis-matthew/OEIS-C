/*
 * A212560  Number of (w,x,y,z) with all terms in {1,...,n} and w+x<=y+z.
 * https://oeis.org/A212560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 2*n^3 + 3*n^4)/6
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 303, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;
    seq_t ans = (N + (seq_t)2 * n3 + (seq_t)3 * n4) / (seq_t)6;
    return ans;
}
