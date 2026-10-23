/*
 * A139482  Binomial transform of [1, 1, 2, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1,
 *          ...].
 * https://oeis.org/A139482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10 - 9*n + 3*n^2)/2 , for n >= 2 with a(1) = 1
 * Range:     u8 n <= 10, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t sn = (seq_t)n;          /* promote index to seq_t */
    seq_t sq = sn * sn;           /* n^2 */
    seq_t term = sq * (seq_t)3;   /* 3*n^2 */
    term = term - (sn * (seq_t)9);/* 3*n^2 - 9*n */
    term = term + (seq_t)10;      /* +10 */
    return term / (seq_t)2;       /* divide by 2 */
}
