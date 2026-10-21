/*
 * A230628  Maximum number of colors needed to color a planar map of several
 *          empires, each empire consisting of n countries.
 * https://oeis.org/A230628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1) = 4; for n > 1, a(n) = 6*n
 * Range:     u8 n <= 42, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)4;
    return (seq_t)n * ((seq_t)6);
}
