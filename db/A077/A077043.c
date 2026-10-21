/*
 * A077043  "Three-quarter squares": a(n) = n^2 - A002620(n).
 * https://oeis.org/A077043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = ceil(3*n^2/4) = (3*n*n + 3)/4
 * Range:     u8 n <= 9, u16 n <= 294, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;           /* convert index to seq_t */
    seq_t sq = N * N;      /* n^2 */
    seq_t tmp = sq * 3;    /* 3*n^2 */
    tmp = tmp + 3;         /* add 3 to implement ceiling */
    return tmp / 4;        /* divide by 4 */
}
