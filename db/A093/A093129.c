/*
 * A093129  Binomial transform of Fibonacci(2n-1) (A001519).
 * https://oeis.org/A093129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    seq_t prev2 = (seq_t)1; /* a(0) */
    seq_t prev1 = (seq_t)2; /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)5 * (prev1 - prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
