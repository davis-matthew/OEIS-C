/*
 * A040005  Continued fraction for sqrt(8).
 * https://oeis.org/A040005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=2; for n>0, a(n)=4 if n is even, otherwise a(n)=1
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 97 terms through n = 8520 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n == 0) ? (seq_t)2 : ((n & 1ULL) ? (seq_t)1 : (seq_t)4);
}
