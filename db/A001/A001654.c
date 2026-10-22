/*
 * A001654  Golden rectangle numbers: F(n) * F(n+1), where F(n) = A000045(n)
 *          (Fibonacci numbers).
 * https://oeis.org/A001654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n) * F(n+1), where F(n) is the n-th Fibonacci number
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t fib_prev = 0, fib_curr = 1;
    
    for (idx_t i = 1; i <= n; i++) {
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    
    return fib_prev * fib_curr;
}
