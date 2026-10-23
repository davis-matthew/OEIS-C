/*
 * A158732  a(n) = 68*n^2 + 1.
 * https://oeis.org/A158732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 68*n^2 + 1
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 7947, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return ((seq_t)68) * tn * tn + (seq_t)1;
}
