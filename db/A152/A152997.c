/*
 * A152997  Twice 13-gonal numbers: a(n) = n*(11*n - 9).
 * https://oeis.org/A152997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(11*n - 9)
 * Range:     u8 n <= 5, u16 n <= 77, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * ((seq_t)11 * nn - (seq_t)9);
}
