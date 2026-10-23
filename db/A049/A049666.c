/*
 * A049666  a(n) = Fibonacci(5*n)/5.
 * https://oeis.org/A049666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) + a(n-2) with a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t prev1 = (seq_t)1;   /* a(n-1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)((seq_t)11 * prev1 + prev2);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
