/*
 * A017489  a(n) = (11*n + 8)^5.
 * https://oeis.org/A017489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 8)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 6, u64 n <= 647, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)11 * (seq_t)n + (seq_t)8;
    seq_t x2 = x * x;       /* x^2   */
    seq_t x4 = x2 * x2;     /* x^4   */
    return x4 * x;          /* x^5   */
}
