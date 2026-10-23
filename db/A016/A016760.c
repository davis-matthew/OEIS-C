/*
 * A016760  a(n) = (2*n+1)^8.
 * https://oeis.org/A016760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)^8
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 7, u64 n <= 127, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * 2 + 1;   /* compute 2*n+1 */
    seq_t x2 = x * x;             /* (2*n+1)^2 */
    seq_t x4 = x2 * x2;           /* (2*n+1)^4 */
    return x4 * x4;               /* (2*n+1)^8 */
}
