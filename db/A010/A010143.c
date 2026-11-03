/*
 * A010143  Continued fraction for sqrt(58).
 * https://oeis.org/A010143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 7; for n ≥ 1, a(n) = 14 if n mod 7 = 0, otherwise 1
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 93 terms through n = 9668 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)7;
    return (n % 7 == 0) ? (seq_t)14 : (seq_t)1;
}
