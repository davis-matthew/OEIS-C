/*
 * A166941  Product plus sum of four consecutive nonnegative numbers.
 * https://oeis.org/A166941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + 6*n^3 + 11*n^2 + 10*n + 6
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 254, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;          /* Convert index to seq_t */
    seq_t m2 = m * m;            /* n^2 */
    seq_t m3 = m2 * m;           /* n^3 */
    seq_t m4 = m3 * m;           /* n^4 */

    return m4 + 6 * m3 + 11 * m2 + 10 * m + (seq_t)6;
}
