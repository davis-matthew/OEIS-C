/*
 * A040029  Continued fraction for sqrt(35).
 * https://oeis.org/A040029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=5; for n>0, a(n)=1 if n is odd, 10 if n is even
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 82 terms through n = 9779 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)5;
    return (n & 1ULL) ? (seq_t)1 : (seq_t)10;
}
