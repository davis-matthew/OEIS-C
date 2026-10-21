/*
 * A093130  Third binomial transform of Fibonacci(3n).
 * https://oeis.org/A093130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=2, a(n)=10*a(n-1)-20*a(n-2) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    seq_t prev2 = (seq_t)0;
    seq_t prev1 = (seq_t)2;
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)10 * prev1 - (seq_t)20 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
