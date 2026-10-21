/*
 * A099919  a(n) = F(3) + F(6) + F(9) + ... + F(3n), F(n) = Fibonacci numbers
 *          A000045.
 * https://oeis.org/A099919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Fibonacci(3*n + 2) - 1)/2 Recurrence: a(0)=0, a(1)=2,
 *            a(n)=4*a(n-1)+a(n-2)+2 (n>1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)2;

    seq_t prev2 = (seq_t)0;          /* a(n-2)  */
    seq_t prev1 = (seq_t)2;          /* a(n-1)  */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)4 * prev1 + prev2 + (seq_t)2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
