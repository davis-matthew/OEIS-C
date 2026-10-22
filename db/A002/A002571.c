/*
 * A002571  From a definite integral.
 * https://oeis.org/A002571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + Fibonacci(n+1)^2, with a(1)=1, a(2)=5
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 5;
    
    seq_t result = (n % 2 == 1) ? (seq_t)1 : (seq_t)5;
    
    seq_t fib_a = 1, fib_b = 1;
    idx_t fib_idx = 2;
    
    idx_t start = (n % 2 == 1) ? 3 : 4;
    for (idx_t i = start; i <= n; i += 2) {
        while (fib_idx < i + 1) {
            seq_t fib_next = fib_a + fib_b;
            fib_a = fib_b;
            fib_b = fib_next;
            fib_idx++;
        }
        result += fib_b * fib_b;
    }
    
    return result;
}
