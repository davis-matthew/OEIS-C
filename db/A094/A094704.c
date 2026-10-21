/*
 * A094704  Convolution of Fibonacci(n) and 10^n.
 * https://oeis.org/A094704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-1) + Fibonacci(n) for n ≥ 1, a(0) = 0
 *            Fibonacci(0)=0, Fibonacci(1)=1. This is an O(n) iterative
 *            computation that works for any seq_t type that supports + and *
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t fib_prev = (seq_t)0;   /* Fibonacci(n-2)  */
    seq_t fib_curr = (seq_t)1;   /* Fibonacci(n-1)  */
    seq_t a_prev   = (seq_t)0;   /* a(n-2)          */
    seq_t a_curr   = (seq_t)1;   /* a(n-1)          */

    /* n == 1 is already handled */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t fib_next = fib_prev + fib_curr;         /* Fibonacci(i)   */
        seq_t a_next   = a_curr * (seq_t)10 + fib_next; /* a(i)   */
        /* shift for next iteration */
        fib_prev = fib_curr;
        fib_curr = fib_next;
        a_prev   = a_curr;
        a_curr   = a_next;
    }
    return a_curr;
}
