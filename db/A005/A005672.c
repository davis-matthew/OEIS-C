/*
 * A005672  a(n) = Fibonacci(n+1) - 2^floor(n/2).
 * https://oeis.org/A005672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+1) - 2^floor(n/2)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Compute Fibonacci(n+1)
    seq_t fib_prev = 0, fib_curr = 1;
    for (idx_t i = 0; i < n; i++) {
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    
    // Compute 2^floor(n/2)
    seq_t power_of_2 = (seq_t)1 << (n / 2);
    
    // Return the difference
    return fib_curr - power_of_2;
}
