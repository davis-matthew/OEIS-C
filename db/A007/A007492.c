/*
 * A007492  Fibonacci(n) - (-1)^n.
 * https://oeis.org/A007492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) - (-1)^n
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t fib_prev = 0, fib_curr = 1;
    for (idx_t i = 2; i <= n; i++) {
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    
    if (n % 2 == 1) {
        return fib_curr + 1;
    } else {
        return fib_curr - 1;
    }
}
