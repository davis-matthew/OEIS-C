/*
 * A152161  a(n) = 100*n^2 + 100*n + 21.
 * https://oeis.org/A152161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*n^2 + 100*n + 21
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 6553, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return (seq_t)100 * tn * tn + (seq_t)100 * tn + (seq_t)21;
}
