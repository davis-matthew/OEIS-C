/*
 * A040063  Continued fraction for sqrt(72).
 * https://oeis.org/A040063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=8; for n>=1, a(n)=2 if n is odd else 16 (period 2: 2,16)
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 82 terms through n = 9906 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)8;
    return ((n & 1ULL) ? (seq_t)2 : (seq_t)16);
}
