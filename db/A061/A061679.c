/*
 * A061679  Concatenation of n^3 and 7.
 * https://oeis.org/A061679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 10*n^3 + 7
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 754, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * nn * nn * (seq_t)10 + (seq_t)7;
}
