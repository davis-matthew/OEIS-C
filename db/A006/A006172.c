/*
 * A006172  a(n) = 1 + F(2*n+1) + (-1)^n*L(n).
 * https://oeis.org/A006172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + F(2*n+1) + (-1)^n*L(n)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Compute F(2*n+1) using iteration
    seq_t fib_prev = 0;  // F(0)
    seq_t fib_curr = 1;  // F(1)
    
    for (idx_t i = 2; i <= 2*n+1; i++) {
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    seq_t f_2n_plus_1 = fib_curr;
    
    // Compute L(n) using iteration
    seq_t luc_prev = 2;  // L(0)
    seq_t luc_curr = 1;  // L(1)
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t luc_next = luc_prev + luc_curr;
        luc_prev = luc_curr;
        luc_curr = luc_next;
    }
    seq_t l_n = (n == 0) ? 2 : luc_curr;
    
    // a(n) = 1 + F(2*n+1) + (-1)^n*L(n)
    if (n % 2 == 0) {
        return 1 + f_2n_plus_1 + l_n;
    } else {
        return 1 + f_2n_plus_1 - l_n;
    }
}
