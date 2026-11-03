/*
 * A144396  The odd numbers greater than 1.
 * https://oeis.org/A144396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2*n + 1
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 880 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)2 * (seq_t)n + 1;
}
