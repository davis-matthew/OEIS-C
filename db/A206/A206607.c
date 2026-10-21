/*
 * A206607  Fibonacci sequence beginning 13, 11.
 * https://oeis.org/A206607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*Fibonacci(n) - 2*Fibonacci(n-1) Recurrence: a(n) =
 *            a(n-1) + a(n-2), a(1) = 13, a(2) = 11
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute Fibonacci(n) and Fibonacci(n-1) iteratively */
    seq_t f_prev = 0; /* Fib(0) */
    seq_t f_cur  = 1; /* Fib(1) */

    /* If n == 1, loop body is skipped and we have Fib(1)=1, Fib(0)=0 */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t f_next = f_prev + f_cur;
        f_prev = f_cur;
        f_cur  = f_next;
    }

    /* compute a(n) = 13*f_cur - 2*f_prev as unsigned arithmetic */
    return 13 * f_cur - 2 * f_prev;
}
