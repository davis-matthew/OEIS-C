/*
 * A007887  a(n) = Fibonacci(n) mod 9.
 * https://oeis.org/A007887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) mod 9
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t fib_prev = 0, fib_curr = 1;
    
    for (idx_t i = 0; i < n; i++) {
        seq_t temp = (fib_prev + fib_curr) % 9;
        fib_prev = fib_curr;
        fib_curr = temp;
    }
    
    return fib_prev;
}
