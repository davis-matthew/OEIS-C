/*
 * A069173  Centered 22-gonal numbers.
 * https://oeis.org/A069173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 11*n^2 - 11*n + 1
 * Range:     u8 n <= 5, u16 n <= 77, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 11 * nn * nn - 11 * nn + 1;
}
