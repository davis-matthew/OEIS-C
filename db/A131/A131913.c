/*
 * A131913  Product of the square matrix in A065941 and the column vector (1,
 *          2, 3, ...)'.
 * https://oeis.org/A131913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*Fibonacci(n), a(0)=1, a(1)=3;
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)3;   /* a(1) */

    seq_t fib_prev = (seq_t)0;  /* F(0) */
    seq_t fib_curr = (seq_t)1;  /* F(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t fib_next = fib_prev + fib_curr;           /* F(i)   */
        seq_t a_i = a_prev1 + a_prev2 + (fib_next << 1); /* a(i) */

        a_prev2 = a_prev1;
        a_prev1 = a_i;

        fib_prev = fib_curr;
        fib_curr = fib_next;
    }

    return a_prev1;
}
