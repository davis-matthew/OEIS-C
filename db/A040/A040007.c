/*
 * A040007  Continued fraction for sqrt(11).
 * https://oeis.org/A040007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=3, a(1)=3, a(n)=3 if n%2==1 else 6 for n>=2
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 96 terms through n = 9714 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)3;
    return (n & 1) ? (seq_t)3 : (seq_t)6;
}
