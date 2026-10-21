/*
 * A090386  Fifth diagonal (m=4) of triangle A084938; a(n) = A084938(n+4,n) =
 *          (n^4 + 18*n^3 + 131*n^2 + 426*n)/24.
 * https://oeis.org/A090386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^4 + 18*n^3 + 131*n^2 + 426*n)/24
 * Range:     u8 n <= 0, u16 n <= 11, u32 n <= 473, u64 n <= 1000,
 *            u128 n <= 1000, big any n
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
    seq_t num = n4 + (seq_t)18 * n3 + (seq_t)131 * n2 + (seq_t)426 * N;
    return num / (seq_t)24;
}
