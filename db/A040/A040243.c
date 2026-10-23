/*
 * A040243  Continued fraction for sqrt(260).
 * https://oeis.org/A040243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 16, 8, 32, 8, 32, ... (continued fraction for sqrt(260))
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)16;
    /* For n >= 1: odd indices give 8, even indices give 32. */
    return (n & (idx_t)1) ? (seq_t)8 : (seq_t)32;
}
