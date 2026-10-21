/*
 * A069133  Centered 20-gonal (or icosagonal) numbers.
 * https://oeis.org/A069133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 10*n^2 - 10*n + 1
 * Range:     u8 n <= 5, u16 n <= 81, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return 10 * x * x - 10 * x + 1;
}
