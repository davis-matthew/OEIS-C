/*
 * A141046  a(n) = 4*n^4.
 * https://oeis.org/A141046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = 4*n^4
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 181, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 4 * nn * nn * nn * nn;
}
