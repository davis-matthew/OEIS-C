/*
 * A000584  Fifth powers: a(n) = n^5.
 * https://oeis.org/A000584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^5
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t m2 = m * m;
    seq_t m4 = m2 * m2;
    return m4 * m;
}
