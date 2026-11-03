/*
 * A004116  a(n) = floor((n^2 + 6n - 3)/4).
 * https://oeis.org/A004116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 6n - 3)/4)
 * Range:     u8 n <= 29, u16 n <= 509, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9738 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    return (m * m + 6 * m - 3) / 4;
}
