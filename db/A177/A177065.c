/*
 * A177065  a(n) = (8*n+3)*(8*n+5).
 * https://oeis.org/A177065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 3) * (8*n + 5)
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 931 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    return (t * 8 + 3) * (t * 8 + 5);
}
