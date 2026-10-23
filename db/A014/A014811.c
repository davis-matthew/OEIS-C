/*
 * A014811  a(n) = Sum_{k=1..n-1} ceiling(k^2/n).
 * https://oeis.org/A014811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n-1} ceil(k^2 / n)
 * Range:     u8 n <= 16, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k < n; ++k)
    {
        seq_t kk   = (seq_t)k * (seq_t)k;           /* k^2 */
        seq_t term = (kk + (seq_t)n - 1) / (seq_t)n;/* ceil(k^2/n) */
        sum += term;
    }
    return sum;
}
