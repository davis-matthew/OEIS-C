/*
 * A135400  a(n) = (4*n^4 - 4*n^3 - n^2 + 3*n)/2.
 * https://oeis.org/A135400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^4 - 4*n^3 - n^2 + 3*n)/2
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 212, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n2 * n2;
    seq_t numerator = ((seq_t)4 * n4) - ((seq_t)4 * n3) - n2 + ((seq_t)3 * N);
    return numerator / 2;
}
