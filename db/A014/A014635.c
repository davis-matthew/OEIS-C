/*
 * A014635  a(n) = 2*n*(4*n - 1).
 * https://oeis.org/A014635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(4*n - 1)
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 880, u64 n <= 880,
 *            u128 n <= 880, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return (seq_t)2 * N * ((seq_t)4 * N - (seq_t)1);
}
