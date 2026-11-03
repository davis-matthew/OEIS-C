/*
 * A010122  Continued fraction for sqrt(13).
 * https://oeis.org/A010122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 3; for n>=1, a(n) = 6 if (n-1) mod 5 == 4, else 1
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 107 terms through n = 9766 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)3;
    }
    idx_t r = (n - 1) % 5;
    return (r == 4) ? (seq_t)6 : (seq_t)1;
}
