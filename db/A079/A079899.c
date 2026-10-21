/*
 * A079899  a(1) = 1; a(n) = Fibonacci(n) - Fibonacci(n-1)* a(n-1) if n > 1.
 * https://oeis.org/A079899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Fibonacci(n) - Fibonacci(n-1) * a(n-1) with a(1) = 1.
 *            (Recurrence uses the Fibonacci sequence. 0-indexed Fibonacci:
 *            F(0)=0, F(1)=1.)
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 11, i64 n <= 15, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0u) {
        /* Undefined for n < 1; return 0 as a safe default */
        return (seq_t)0;
    }
    if (n == 1u) {
        return (seq_t)1;
    }
    /* F(0) = 0, F(1) = 1 */
    seq_t fib_prev = (seq_t)0;   /* F(i-2) */
    seq_t fib_cur  = (seq_t)1;   /* F(i-1) */
    seq_t prev_a   = (seq_t)1;   /* a(1) */

    for (idx_t i = 2u; i <= n; ++i) {
        seq_t fib_next = fib_prev + fib_cur;              /* F(i)   */
        seq_t next_a   = fib_next - fib_cur * prev_a;     /* a(i)   */
        /* shift Fibonacci indices for next iteration */
        fib_prev = fib_cur;
        fib_cur  = fib_next;
        prev_a   = next_a;
    }
    return prev_a;
}
