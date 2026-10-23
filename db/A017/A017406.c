/*
 * A017406  a(n) = (11n+1)^6.
 * https://oeis.org/A017406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n+1)^6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 3, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n * 11 + 1;
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    return x3 * x3;
}
