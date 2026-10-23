/*
 * A152746  Six times hexagonal numbers: 6*n*(2*n-1).
 * https://oeis.org/A152746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, walk
 * Formula:   a(n) = 6*n*(2*n-1)
 * Range:     u8 n <= 4, u16 n <= 74, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return ((seq_t)6) * tn * (((seq_t)2 * tn) - (seq_t)1);
}
