/*
 * A178732  a(n) = n XOR 6n, where XOR is bitwise XOR.
 * https://oeis.org/A178732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n XOR (6*n)
 * Range:     u8 n <= 42, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t y = x * 6;
    return x ^ y;
}
