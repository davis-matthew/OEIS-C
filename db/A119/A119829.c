/*
 * A119829  Diagonal sum of number triangle A119828.
 * https://oeis.org/A119829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (2(n-k))!/(2k)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = 1;
    idx_t limit = n << 1;                     /* 2 * n */
    for (idx_t i = 2; i <= limit; ++i)
        t *= (seq_t)i;                         /* compute (2n)! */

    seq_t sum = t;
    idx_t maxk = n >> 1;                       /* floor(n/2) */
    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t denom1 = (seq_t)(2 * k);
        seq_t denom2 = (seq_t)(2 * k - 1);
        seq_t denom3 = (seq_t)(2 * n - 2 * k + 2);
        seq_t denom4 = (seq_t)(2 * n - 2 * k + 1);
        seq_t denom = denom1 * denom2 * denom3 * denom4;
        t = t / denom;
        sum += t;
    }
    return sum;
}
