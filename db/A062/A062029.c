/*
 * A062029  Group even numbers into (2), (4,6), (8,10,12), (14,16,18,20), ...;
 *          a(n) = product of n-th group.
 * https://oeis.org/A062029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (n^2 - n + 2*k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    seq_t base = (seq_t)n * (seq_t)n - (seq_t)n;      /* n^2 - n   */
    for (idx_t k = 1; k <= n; ++k) {
        prod *= base + (seq_t)2 * (seq_t)k;            /* n^2 - n + 2*k */
    }
    return prod;
}
