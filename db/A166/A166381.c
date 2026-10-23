/*
 * A166381  a(n) = Sum_{j>n} floor(n^2/j).
 * https://oeis.org/A166381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} floor(n*(n-i)/i)
 * Range:     u8 n <= 11, u16 n <= 122, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = (seq_t)n * (seq_t)(n - i) / (seq_t)i;
        sum += term;
    }
    return sum;
}
