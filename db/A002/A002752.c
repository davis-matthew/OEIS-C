/*
 * A002752  a(n) = Fibonacci(n-1) mod n.
 * https://oeis.org/A002752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n-1) mod n
 * Range:     u8 n <= 278, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return 0;
    }
    
    idx_t fib_prev = 0;
    idx_t fib_curr = 1;
    
    for (idx_t k = 1; k < n - 1; k++) {
        idx_t fib_next = (fib_prev + fib_curr) % n;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    
    return (seq_t)fib_curr;
}
