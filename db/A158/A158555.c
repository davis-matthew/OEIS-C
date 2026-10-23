/*
 * A158555  a(n) = 196*n^2 + 14.
 * https://oeis.org/A158555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 196*n^2 + 14
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 4681, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return 196 * tn * tn + (seq_t)14;
}
