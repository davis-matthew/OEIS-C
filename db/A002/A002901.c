/*
 * A002901  a(n) = n^3 - floor( n/3 ).
 * https://oeis.org/A002901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 - floor(n/3)
 * Range:     u8 n <= 6, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * nn * nn - nn / 3;
}
