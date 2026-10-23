/*
 * A013655  a(n) = F(n+1) + L(n), where F(n) and L(n) are Fibonacci and Lucas
 *          numbers, respectively.
 * https://oeis.org/A013655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 3, a(1) = 2
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A013655: a(n) = Fibonacci(n+1) + Lucas(n) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)2;

    seq_t prev2 = (seq_t)3;  /* a(0) */
    seq_t prev1 = (seq_t)2;  /* a(1) */
    seq_t curr  = prev1 + prev2;
    for (idx_t i = 2; i <= n; ++i) {
        prev2 = prev1;
        prev1 = curr;
        if (i == n) break;
        curr = prev1 + prev2;
    }
    return prev1;
}
