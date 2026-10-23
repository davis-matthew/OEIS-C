/*
 * A158628  a(n) = 44*n^2 - 1.
 * https://oeis.org/A158628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 44*n^2 - 1
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 9879, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return ((seq_t)44 * nn * nn) - (seq_t)1;
}
