/*
 * A040021  Continued fraction for sqrt(27).
 * https://oeis.org/A040021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 5*(3+(-1)^n)/2 (with a(0) = 5)
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 82 terms through n = 9474 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is a special case */
    if (n == 0)
        return 5;

    /* For n ≥ 1, the parity of n decides the value:
       - odd n → 5
       - even n → 10 */
    return (n & 1ULL) ? 5 : 10;
}
