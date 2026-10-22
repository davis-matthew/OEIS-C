/*
 * A001655  Fibonomial coefficients: a(n) = F(n+1) * F(n+2) * F(n+3)/2, where
 *          F() = Fibonacci numbers A000045.
 * https://oeis.org/A001655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n+1) * F(n+2) * F(n+3) / 2, where F() is Fibonacci
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t fn1, fn2, fn3;
    seq_t f_prev = 0, f_curr = 1;
    
    for (idx_t i = 1; i <= n + 3; i++) {
        if (i == n + 1) fn1 = f_curr;
        if (i == n + 2) fn2 = f_curr;
        if (i == n + 3) fn3 = f_curr;
        
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
    
    return (fn1 * fn2 * fn3) / 2;
}
