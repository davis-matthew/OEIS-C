/*
 * A016916  a(n) = (6*n)^8.
 * https://oeis.org/A016916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n)^8
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 42, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 6;   /* 6*n */
    seq_t m2 = m * m;         /* (6*n)^2 */
    seq_t m4 = m2 * m2;       /* (6*n)^4 */
    seq_t m8 = m4 * m4;       /* (6*n)^8 */
    return m8;
}
