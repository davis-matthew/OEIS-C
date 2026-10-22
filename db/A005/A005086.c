/*
 * A005086  Number of Fibonacci numbers 1,2,3,5,... dividing n.
 * https://oeis.org/A005086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Fibonacci numbers dividing n
 * Range:     u8 n <= 2515, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t count = 0;
    
    seq_t fib_prev = 0;
    seq_t fib_curr = 1;
    seq_t last_checked = 0;
    
    while (fib_curr <= m) {
        if (fib_curr != last_checked && m % fib_curr == 0) {
            count++;
        }
        last_checked = fib_curr;
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    
    return count;
}
