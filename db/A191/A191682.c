/*
 * A191682  Twice A113473.
 * https://oeis.org/A191682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2*(floor(log2(n)) + 1) (= 2*ceil(log2(n+1)))
 * Range:     u8 n <= 31, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1, but harmless */
    idx_t log = 0;                        /* floor(log2(n)) */
    idx_t tmp = n;
    while (tmp >>= 1) ++log;
    return (seq_t)(2 * (log + 1));
}
