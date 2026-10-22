/*
 * A002708  a(n) = Fibonacci(n) mod n.
 * https://oeis.org/A002708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = Fibonacci(n) mod n
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t mod = (seq_t)n;
    seq_t fib_prev = 0;
    seq_t fib_curr = 1 % mod;
    
    for (idx_t i = 1; i < n; i++) {
        seq_t fib_next = (fib_prev + fib_curr) % mod;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    
    return fib_curr;
}
