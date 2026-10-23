/*
 * A100892  a(n) = (2*n-1) XOR (2*n+1), bitwise.
 * https://oeis.org/A100892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-1) XOR (2*n+1)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = ((seq_t)n << 1) - 1;
    seq_t y = ((seq_t)n << 1) + 1;
    return x ^ y;
}
