/*
 * A003266  Product of first n nonzero Fibonacci numbers F(1), ..., F(n).
 * https://oeis.org/A003266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Product of F(1), F(2), ..., F(n), where F(i) is the i-th
 *            Fibonacci number
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t product = 1;
    seq_t fib_prev = 0, fib_curr = 1;
    
    for (idx_t i = 1; i <= n; i++) {
        product *= fib_curr;
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    
    return product;
}
