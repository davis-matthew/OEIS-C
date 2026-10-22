/*
 * A005922  a(1)=1; a(n) = n!*Fibonacci(n+2), n > 1.
 * https://oeis.org/A005922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = n! * Fibonacci(n+2), n > 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t factorial = 1;
    for (idx_t i = 2; i <= n; i++) {
        factorial *= (seq_t)i;
    }
    
    seq_t fib_a = 1, fib_b = 1;
    for (idx_t i = 3; i <= n + 2; i++) {
        seq_t temp = fib_a + fib_b;
        fib_a = fib_b;
        fib_b = temp;
    }
    
    return factorial * fib_b;
}
