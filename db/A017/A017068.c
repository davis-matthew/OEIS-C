/*
 * A017068  a(n) = (8*n)^4.
 * https://oeis.org/A017068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 31, u64 n <= 8191,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    x *= x;     /* n^2 */
    x *= x;     /* n^4 */
    x *= 4096;  /* 8^4 = 4096 */
    return x;
}
