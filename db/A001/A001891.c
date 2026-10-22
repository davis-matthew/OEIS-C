/*
 * A001891  Hit polynomials; convolution of natural numbers with Fibonacci
 *          numbers F(2), F(3), F(4), ....
 * https://oeis.org/A001891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+5) - (5+2*n)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t target = n + 5;
    seq_t fib_prev, fib_curr;
    
    if (target <= 1) {
        fib_curr = (seq_t)target;
    } else {
        fib_prev = 0;
        fib_curr = 1;
        
        for (idx_t i = 2; i <= target; i++) {
            seq_t fib_next = fib_prev + fib_curr;
            fib_prev = fib_curr;
            fib_curr = fib_next;
        }
    }
    
    return fib_curr - (seq_t)5 - (seq_t)n * (seq_t)2;
}
