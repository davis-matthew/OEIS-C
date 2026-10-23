/*
 * A016830  a(n) = (4*n+2)^6.
 * https://oeis.org/A016830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+2)^6
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 9, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * 4 + 2;   /* 4*n + 2 */
    seq_t x2 = x * x;            /* (4*n+2)^2 */
    seq_t x4 = x2 * x2;          /* (4*n+2)^4 */
    return x4 * x2;              /* (4*n+2)^6 = (4*n+2)^4 * (4*n+2)^2 */
}
