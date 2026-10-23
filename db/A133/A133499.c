/*
 * A133499  a(n) = n^7 - n.
 * https://oeis.org/A133499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 - n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 565, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;     /* n */
    seq_t x2 = x * x;        /* n^2 */
    seq_t x3 = x2 * x;       /* n^3 */
    seq_t x4 = x2 * x2;      /* n^4 */
    seq_t x7 = x4 * x3;      /* n^7 */
    return x7 - x;           /* n^7 - n */
}
