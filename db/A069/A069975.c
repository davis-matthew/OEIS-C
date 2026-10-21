/*
 * A069975  a(n) = n*(16*n^2 - 1).
 * https://oeis.org/A069975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(16*n^2 - 1)
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    return nn * ((seq_t)16 * nn * nn - 1);
}
