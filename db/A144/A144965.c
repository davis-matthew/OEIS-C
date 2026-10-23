/*
 * A144965  a(n) = 4*n*(4*n^2 + 1).
 * https://oeis.org/A144965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*n*(4*n^2 + 1)
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 645, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t) n;
    return 4 * tn * (4 * tn * tn + 1);
}
