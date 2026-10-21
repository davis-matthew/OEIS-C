/*
 * A220083  a(n) = (15*n^2 + 9*n + 2)/2.
 * https://oeis.org/A220083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15*n^2 + 9*n + 2)/2
 * Range:     u8 n <= 3, u16 n <= 92, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* Compute numerator using seq_t arithmetic to avoid overflow in
       intermediate integer types. */
    seq_t numerator = (seq_t)15 * N * N + (seq_t)9 * N + (seq_t)2;
    return numerator / (seq_t)2;
}
