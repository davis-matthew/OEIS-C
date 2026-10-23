/*
 * A175829  Partial sums of ceiling(n^2/11).
 * https://oeis.org/A175829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1}^n ceil(k^2/11)
 * Range:     u8 n <= 15, u16 n <= 128, u32 n <= 870, u64 n <= 870,
 *            u128 n <= 870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t sq = (seq_t)k * (seq_t)k;         /* k^2 */
        sum += (sq + (seq_t)10) / (seq_t)11;    /* ceil(k^2 / 11) */
    }
    return sum;
}
