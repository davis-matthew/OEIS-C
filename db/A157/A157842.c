/*
 * A157842  a(n) = 3600*n^2 - 5599*n + 2177.
 * https://oeis.org/A157842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3600*n^2 - 5599*n + 2177
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 1093, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;              /* cast index to seq_t */
    seq_t val = 3600 * nn * nn        /* 3600*n^2   */
              - 5599 * nn             /* -5599*n     */
              + 2177;                 /* +2177       */
    return val;
}
