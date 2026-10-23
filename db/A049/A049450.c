/*
 * A049450  Pentagonal numbers multiplied by 2: a(n) = n*(3*n-1).
 * https://oeis.org/A049450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(3*n-1)
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * ((seq_t)3 * nn - (seq_t)1);
}
