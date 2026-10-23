/*
 * A158070  a(n) = 64*n^2 + 2*n.
 * https://oeis.org/A158070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64*n^2 + 2*n
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 8191, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return ((seq_t)64 * nn * nn) + ((seq_t)2 * nn);
}
