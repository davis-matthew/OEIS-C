/*
 * A008346  a(n) = Fibonacci(n) + (-1)^n.
 * https://oeis.org/A008346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) + (-1)^n
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases that avoid the loop overhead */
    if (n == 0U)
        return (seq_t)1;   /* Fib(0) + 1 = 0 + 1 = 1 */
    if (n == 1U)
        return (seq_t)0;   /* Fib(1) + (-1) = 1 - 1 = 0 */

    /* Compute Fibonacci(n) iteratively for n >= 2 */
    seq_t prev = 1;  /* Fib(1) */
    seq_t curr = 1;  /* Fib(2) */
    idx_t i;

    for (i = 3; i <= n; ++i) {
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }
    /* now curr == Fib(n) */

    /* Add (-1)^n : add 1 if n is even, subtract 1 if n is odd */
    return ((n & 1U) == 0U) ? curr + 1 : curr - 1;
}
