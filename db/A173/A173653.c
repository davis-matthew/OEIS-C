/*
 * A173653  Partial sums of floor(n^2/10) (A056865).
 * https://oeis.org/A173653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0}^{n} floor(k^2/10)
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        seq_t sq = (seq_t)i * (seq_t)i;   /* k^2 */
        sum += sq / 10;                    /* floor(k^2/10) */
    }
    return sum;
}
