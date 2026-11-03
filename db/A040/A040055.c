/*
 * A040055  Continued fraction for sqrt(63).
 * https://oeis.org/A040055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(n) = 7, 1, 14, 1, 14, ... (continued fraction for sqrt(63))
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 82 terms through n = 9107 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The first term is the integer part of sqrt(63). */
    if (n == 0)
        return (seq_t)7;
    /* Subsequent terms alternate 1 and 14. */
    return (n & 1) ? (seq_t)1 : (seq_t)14;
}
