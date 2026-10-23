/*
 * A163303  a(n) = n^3 + 73*n^2 + n + 67.
 * https://oeis.org/A163303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + 73*n^2 + n + 67
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 1601, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    return n3 + (seq_t)73 * n2 + nn + (seq_t)67;
}
