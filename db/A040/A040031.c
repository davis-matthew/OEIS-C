/*
 * A040031  Continued fraction for sqrt(38).
 * https://oeis.org/A040031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 6 for n = 0 or n = 1; a(n) = 12 if n ≥ 2 and n is even;
 *            a(n) = 6 if n ≥ 2 and n is odd
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 82 terms through n = 9876 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)6;
    return (n & 1) ? (seq_t)6 : (seq_t)12;
}
