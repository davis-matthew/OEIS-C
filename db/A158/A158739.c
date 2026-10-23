/*
 * A158739  a(n) = 1296*n^2 + 36.
 * https://oeis.org/A158739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1296*n^2 + 36
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 1820, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;          /* convert index to seq_t safely           */
    seq_t sq = tn * tn;           /* n^2                                        */
    seq_t res = sq * (seq_t)1296; /* 1296 * n^2                                 */
    res += (seq_t)36;             /* +36                                        */
    return res;
}
