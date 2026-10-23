/*
 * A100104  a(n) = n^3 - n^2 + 1.
 * https://oeis.org/A100104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 - n^2 + 1
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return x*x*x - x*x + 1;
}
