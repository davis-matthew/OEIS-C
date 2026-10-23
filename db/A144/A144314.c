/*
 * A144314  a(n) = 3*n*(6*n + 1).
 * https://oeis.org/A144314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(6*n + 1)
 * Range:     u8 n <= 3, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return (seq_t)3 * N * ((seq_t)6 * N + (seq_t)1);
}
