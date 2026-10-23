/*
 * A022362  Fibonacci sequence beginning 0, 28.
 * https://oeis.org/A022362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28 * Fib(n) with Fib(0)=0, Fib(1)=1 Recurrence: a(0)=0,
 *            a(1)=28, a(n)=a(n-1)+a(n-2)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two indices directly */
    if (n == 0) return 0;
    if (n == 1) return (seq_t)28;

    /* Compute Fibonacci numbers multiplied by 28 on the fly */
    seq_t prev2 = 0;          /* a(n-2)  */
    seq_t prev1 = 28;         /* a(n-1)  */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 + prev2;  /* a(n)   */
        prev2 = prev1;        /* shift for next iteration */
        prev1 = cur;
    }
    return prev1;
}
