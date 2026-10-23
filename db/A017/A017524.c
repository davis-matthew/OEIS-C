/*
 * A017524  a(n) = (12*n)^4.
 * https://oeis.org/A017524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 21, u64 n <= 5461,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)12 * (seq_t)n; /* t = 12*n */
    t *= t;                         /* t = (12*n)^2 */
    t *= t;                         /* t = (12*n)^4 */
    return t;
}
