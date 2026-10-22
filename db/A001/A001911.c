/*
 * A001911  a(n) = Fibonacci(n+3) - 2.
 * https://oeis.org/A001911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = Fibonacci(n+3) - 2
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t fib_prev = 0;
    seq_t fib_curr = 1;
    
    for (idx_t i = 0; i < n + 2; i++) {
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    
    return fib_curr - 2;
}
