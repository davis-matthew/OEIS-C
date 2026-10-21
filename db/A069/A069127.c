/*
 * A069127  Centered 14-gonal numbers.
 * https://oeis.org/A069127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 7*n^2 - 7*n + 1
 * Range:     u8 n <= 6, u16 n <= 97, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    return 7 * nn * (nn - 1) + 1;
}
