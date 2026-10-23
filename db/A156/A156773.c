/*
 * A156773  a(n) = 6561*n^2 - 9558*n + 3482.
 * https://oeis.org/A156773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6561*n^2 - 9558*n + 3482
 * Range:     u8 none, u16 n <= 3, u32 n <= 809, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;
    return ((seq_t)6561 * N * N + (seq_t)3482) - (seq_t)9558 * N;
}
