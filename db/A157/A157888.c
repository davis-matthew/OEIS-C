/*
 * A157888  a(n) = 81*n^2 + 9.
 * https://oeis.org/A157888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 81*n^2 + 9
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 7281, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return ((seq_t)81 * nn * nn) + 9;
}
