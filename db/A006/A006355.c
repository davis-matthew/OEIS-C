/*
 * A006355  Number of binary vectors of length n containing no singletons.
 * https://oeis.org/A006355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(0) = 1; a(n) = 2*F(n-1) for n > 0, where F(n) is Fibonacci
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    
    seq_t fib_prev = 0;
    seq_t fib_curr = 1;
    
    for (idx_t i = 2; i < n; i++) {
        seq_t temp = fib_curr + fib_prev;
        fib_prev = fib_curr;
        fib_curr = temp;
    }
    
    return 2 * fib_curr;
}
