/*
 * A051869  17-gonal (or heptadecagonal) numbers: a(n) = n*(15*n-13)/2.
 * https://oeis.org/A051869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(15*n-13)/2
 * Range:     u8 n <= 6, u16 n <= 93, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t temp = ((seq_t)15 * nn) - (seq_t)13;
    return (nn * temp) / (seq_t)2;
}
