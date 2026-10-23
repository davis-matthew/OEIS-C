/*
 * A143166  a(n) = n*(8*n^2 + 1)/3.
 * https://oeis.org/A143166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(8*n^2 + 1)/3
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 813, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* int to seq_t */
    seq_t sq = nn * nn;           /* n^2 */
    seq_t v  = sq * (seq_t)8;     /* 8*n^2 */
    v += (seq_t)1;                /* 8*n^2 + 1 */
    v *= nn;                      /* n*(8*n^2 + 1) */
    v /= (seq_t)3;                /* divide by 3 */
    return v;
}
