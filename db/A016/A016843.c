/*
 * A016843  a(n) = (4n+3)^7.
 * https://oeis.org/A016843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n+3)^7
 * Range:     u8 none, u16 n <= 0, u32 n <= 5, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)4 * (seq_t)n + (seq_t)3;  /* base 4n+3 */
    seq_t y = x * x;          /* x² */
    y = y * y;                /* x⁴ */
    y = y * x;                /* x⁵ */
    y = y * x;                /* x⁶ */
    y = y * x;                /* x⁷ */
    return y;
}
