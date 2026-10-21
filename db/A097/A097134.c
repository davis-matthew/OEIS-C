/*
 * A097134  a(n) = 3*Fibonacci(2*n) + 0^n.
 * https://oeis.org/A097134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula used: a(n) = 3*a(n-1) - a(n-2)   (n > 2)
   Initial values:  a(0) = 1,  a(1) = 3,  a(2) = 9
   This recurrence follows from the definition a(n) = 3*Fibonacci(2*n) + 0^n.
   It is linear and can be computed in O(n) time with O(1) memory. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;
    if (n == (idx_t)1)
        return (seq_t)3;
    if (n == (idx_t)2)
        return (seq_t)9;

    seq_t prev2 = (seq_t)3;   /* a(1) */
    seq_t prev1 = (seq_t)9;   /* a(2) */
    seq_t curr;

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        curr = (seq_t)3 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
