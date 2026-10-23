/*
 * A126532  Number of base 11 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{d=0..10} f(n,d) where f(1,d) = 1 for all d, and
 *            f(k+1,d) = sum_{|d - k| <= 5} f(k,k). a(0) = 1, a(1) = 11
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;
    if (n == (idx_t)1)
        return (seq_t)11;

    /* dp[d] holds f(current_length,d) */
    seq_t dp[11], next[11];

    /* initial length 1: all digits possible */
    for (idx_t d = 0; d <= 10; ++d)
        dp[d] = (seq_t)1;

    for (idx_t cur = 2; cur <= n; ++cur) {
        for (idx_t d = 0; d <= 10; ++d) {
            seq_t sum = (seq_t)0;
            idx_t lo = (d > 5) ? (d - 5) : 0;
            idx_t hi = (d + 5 < 11) ? (d + 5) : 10;
            for (idx_t k = lo; k <= hi; ++k)
                sum += dp[k];
            next[d] = sum;
        }
        /* prepare for next iteration */
        for (idx_t d = 0; d <= 10; ++d)
            dp[d] = next[d];
    }

    /* total for length n */
    seq_t total = (seq_t)0;
    for (idx_t d = 0; d <= 10; ++d)
        total += dp[d];
    return total;
}
