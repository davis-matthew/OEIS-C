/*
 * A040031  Continued fraction for sqrt(38).
 * https://oeis.org/A040031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 6 for n = 0 or n = 1; a(n) = 12 if n ≥ 2 and n is even;
 *            a(n) = 6 if n ≥ 2 and n is odd
 * Range:     u8 n <= 9876, u16 n <= 9876, u32 n <= 9876, u64 n <= 9876,
 *            u128 n <= 9876, big any n
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
