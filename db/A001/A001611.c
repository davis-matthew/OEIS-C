/*
 * A001611  a(n) = Fibonacci(n) + 1.
 * https://oeis.org/A001611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, hear
 * Formula:   a(n) = Fibonacci(n) + 1
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    
    seq_t fib_prev2 = (seq_t)0;
    seq_t fib_prev1 = (seq_t)1;
    seq_t fib_curr;
    
    for (idx_t i = 2; i <= n; i++) {
        fib_curr = fib_prev2 + fib_prev1;
        fib_prev2 = fib_prev1;
        fib_prev1 = fib_curr;
    }
    
    return fib_curr + (seq_t)1;
}
