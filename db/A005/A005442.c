/*
 * A005442  a(n) = n!*Fibonacci(n+1).
 * https://oeis.org/A005442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Fibonacci(n+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; i++) {
        fact *= (seq_t)i;
    }
    
    seq_t fib_a = 0;
    seq_t fib_b = 1;
    for (idx_t i = 0; i < n; i++) {
        seq_t temp = fib_a + fib_b;
        fib_a = fib_b;
        fib_b = temp;
    }
    
    return fact * fib_b;
}
