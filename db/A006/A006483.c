/*
 * A006483  a(n) = Fibonacci(n)*2^n + 1.
 * https://oeis.org/A006483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n)*2^n + 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t fib_a = 0, fib_b = 1;
    seq_t pow2 = 1;
    
    for (idx_t i = 0; i < n; i++) {
        seq_t temp = fib_a + fib_b;
        fib_a = fib_b;
        fib_b = temp;
        pow2 *= 2;
    }
    
    return fib_a * pow2 + 1;
}
