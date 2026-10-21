/*
 * A087426  a(n) = S(n,4) where S(n,m) = Sum_{k=0..n}
 *          binomial(n,k)*Fibonacci(m*k).
 * https://oeis.org/A087426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1)-9*a(n-2), a(0)=0, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 3;
    seq_t prev2 = 0;   /* a(n-2) */
    seq_t prev1 = 3;   /* a(n-1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = (prev1 - prev2) * 9;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
