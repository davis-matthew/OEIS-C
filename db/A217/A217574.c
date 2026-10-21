/*
 * A217574  a(n) = (n^2)*(n^2-1)*(n^2-2)*(n^2-3).
 * https://oeis.org/A217574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (n^2-1) * (n^2-2) * (n^2-3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 256, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the expression contains a zero factor */
    if (n < 2)
        return (seq_t)0;

    seq_t ns = (seq_t)n;
    seq_t sq = ns * ns;                 /* n^2 */
    seq_t res = sq * (sq - 1) * (sq - 2) * (sq - 3);
    return res;
}
