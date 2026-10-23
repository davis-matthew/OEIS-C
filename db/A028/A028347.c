/*
 * A028347  a(n) = n^2 - 4.
 * https://oeis.org/A028347
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n*n - 4
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * nn - (seq_t)4;
}
