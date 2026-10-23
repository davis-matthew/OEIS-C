/*
 * A179436  a(n) = (3*n+7)*(3*n+2)/2.
 * https://oeis.org/A179436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+7)*(3*n+2)/2
 * Range:     u8 n <= 6, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = nn * 3 + 7;   /* 3*n + 7 */
    seq_t t2 = nn * 3 + 2;   /* 3*n + 2 */
    return t1 * t2 / 2;
}
