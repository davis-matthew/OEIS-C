/*
 * A016986  a(n) = (7*n)^6.
 * https://oeis.org/A016986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n)^6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 5, u64 n <= 232, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n * 7;
    seq_t x2 = x * x;     /* (7n)^2 */
    seq_t x4 = x2 * x2;   /* (7n)^4 */
    seq_t x6 = x4 * x2;   /* (7n)^6 */
    return x6;
}
