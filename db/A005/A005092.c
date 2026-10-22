/*
 * A005092  Sum of Fibonacci numbers 1,2,3,5,... that divide n.
 * https://oeis.org/A005092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of Fibonacci numbers (1, 2, 3, 5, 8, ...) that divide
 *            n
 * Range:     u8 n <= 375, u16 n <= 9977, u32 n <= 9977, u64 n <= 9977,
 *            u128 n <= 9977, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;  // 1 always divides n
    
    // Check Fibonacci numbers starting from 2
    seq_t fib_prev = 1, fib_curr = 2;
    while (fib_curr <= (seq_t)n) {
        if (((seq_t)n) % fib_curr == 0) {
            result += fib_curr;
        }
        seq_t next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = next;
    }
    
    return result;
}
