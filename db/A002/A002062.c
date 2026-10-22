/*
 * A002062  a(n) = Fibonacci(n) + n.
 * https://oeis.org/A002062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Fibonacci(n) + n
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;
    }
    
    seq_t fib_prev = (seq_t)0;
    seq_t fib_curr = (seq_t)1;
    
    for (idx_t i = 1; i < n; i++) {
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    
    return fib_curr + (seq_t)n;
}
