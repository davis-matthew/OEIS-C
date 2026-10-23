/*
 * A105374  a(n) = 4*n^3 + 4*n.
 * https://oeis.org/A105374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*n^3 + 4*n
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;
    seq_t res = ((seq_t)4 * N * N * N) + ((seq_t)4 * N);
    return res;
}
