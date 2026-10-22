/*
 * A005921  From solution to a difference equation.
 * https://oeis.org/A005921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = Fibonacci(n+3)*n! for n >= 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t factorial = 1;
    for (idx_t i = 1; i <= n; i++) {
        factorial *= i;
    }
    
    seq_t fib_prev = 0, fib_curr = 1;
    for (idx_t i = 2; i <= n + 3; i++) {
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    
    return fib_curr * factorial;
}
