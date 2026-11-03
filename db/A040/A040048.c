/*
 * A040048  Continued fraction for sqrt(56).
 * https://oeis.org/A040048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 82 terms through n = 9889 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=7; for n>0: a(n)=2 if n is odd; a(n)=14 if n is even */

seq_t a(idx_t n)
{
    if (n == 0)
        return 7;
    return (n & 1) ? 2 : 14;
}
