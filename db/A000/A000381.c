/*
 * A000381  Essentially the same as A001611.
 * https://oeis.org/A000381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = F(n+2) + 1, where F(k) is the k-th Fibonacci number with
 *            F(0)=0, F(1)=1
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 18, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t k = n + 2;
    
    seq_t fib_prev = (seq_t)0;
    seq_t fib_curr = (seq_t)1;
    
    for (idx_t i = 2; i <= k; i++) {
        seq_t temp = fib_curr + fib_prev;
        fib_prev = fib_curr;
        fib_curr = temp;
    }
    
    return fib_curr + 1;
}
