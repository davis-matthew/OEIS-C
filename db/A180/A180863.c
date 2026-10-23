/*
 * A180863  Wiener index of the n-sun graph.
 * https://oeis.org/A180863
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(4*n-5)
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (((seq_t)4 * nn) - ((seq_t)5));
}
