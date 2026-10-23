/*
 * A016766  a(n) = (3*n)^2.
 * https://oeis.org/A016766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n)^2 (i.e., a(n) = 9*n^2)
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * nn * 9;
}
