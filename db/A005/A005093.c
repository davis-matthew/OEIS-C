/*
 * A005093  Sum of squares of Fibonacci numbers 1,2,3,5,... that divide n.
 * https://oeis.org/A005093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of Fib(k)^2 for all k >= 2 such that Fib(k) divides n
 * Range:     u8 n <= 20, u16 n <= 376, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t n_seq = (seq_t)n;
    
    result += (seq_t)1;
    
    seq_t fib_prev = 1, fib_curr = 2;
    
    while (fib_curr <= n_seq) {
        if (n_seq % fib_curr == 0) {
            result += fib_curr * fib_curr;
        }
        
        seq_t fib_next = fib_prev + fib_curr;
        if (fib_next < fib_curr) break;
        
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    
    return result;
}
