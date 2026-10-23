/*
 * A017177  a(n) = (9n+1)^5.
 * https://oeis.org/A017177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9n+1)^5
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 9, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 9 * (seq_t)n + 1;
    seq_t r = x;
    r *= x; /* x^2 */
    r *= x; /* x^3 */
    r *= x; /* x^4 */
    r *= x; /* x^5 */
    return r;
}
