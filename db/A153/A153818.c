/*
 * A153818  a(n) = Sum_{k=1..n} floor(n^2/k^2).
 * https://oeis.org/A153818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n^2/k^2)
 * Range:     u8 n <= 12, u16 n <= 200, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t sq  = (seq_t)n * (seq_t)n;   /* n^2, fit in seq_t for n up to ~10^9 */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t d = (seq_t)k * (seq_t)k; /* k^2 */
        sum += sq / d;
    }
    return sum;
}
