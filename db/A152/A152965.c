/*
 * A152965  Twice 12-gonal numbers: a(n) = 2*n*(5*n-4).
 * https://oeis.org/A152965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*(5*n-4)
 * Range:     u8 n <= 5, u16 n <= 81, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t term = 2 * (N * (5 * N - 4));
    return term;
}
