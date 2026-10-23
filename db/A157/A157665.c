/*
 * A157665  a(n) = 729*n^2 - 1016*n + 354.
 * https://oeis.org/A157665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 729*n*n - 1016*n + 354
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 2427, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t res = nn * 729;                   /* 729*n^2 */
    res -= (seq_t)1016 * (seq_t)n;          /* subtract 1016*n */
    res += 354;                             /* add constant */
    return res;
}
