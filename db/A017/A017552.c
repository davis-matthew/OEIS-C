/*
 * A017552  a(n) = (12*n + 2)^8.
 * https://oeis.org/A017552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 2)^8
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 21, u128 n <= 5461,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = ((seq_t)12 * (seq_t)n) + (seq_t)2;
    seq_t y = x * x; /* x^2 */
    y *= y;          /* x^4 */
    y *= y;          /* x^8 */
    return y;
}
