/*
 * A040002  Continued fraction for sqrt(5).
 * https://oeis.org/A040002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, cons
 * Formula:   a(0)=2, a(n)=4 for n>0
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 97 terms through n = 9480 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    return (seq_t)4;
}
