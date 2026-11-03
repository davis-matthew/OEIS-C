/*
 * A106648  a(n) = 3*n^2 + 6*n + 8.
 * https://oeis.org/A106648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 + 6*n + 8
 * Range:     u8 n <= 8, u16 n <= 146, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 976 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return 3 * x * x + 6 * x + 8;
}
