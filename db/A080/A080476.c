/*
 * A080476  Floor( geometric mean of next n numbers ).
 * https://oeis.org/A080476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, for n > 1 a(n) = floor(n² / 2)
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    seq_t sq = (seq_t)n * (seq_t)n;
    return sq / 2;
}
