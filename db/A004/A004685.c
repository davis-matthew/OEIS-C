/*
 * A004685  Fibonacci numbers written in base 2.
 * https://oeis.org/A004685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A007088(A000045(n)) - Fibonacci F(n) with binary
 *            representation interpreted as decimal
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Compute F(n)
    seq_t fib_prev = 0, fib_curr = 1;
    
    if (n == 0) {
        fib_curr = 0;
    } else {
        for (idx_t i = 1; i < n; i++) {
            seq_t fib_next = fib_prev + fib_curr;
            fib_prev = fib_curr;
            fib_curr = fib_next;
        }
    }
    
    // Convert binary representation to decimal number
    seq_t result = 0;
    seq_t power = 1;
    seq_t temp = fib_curr;
    
    while (temp > 0) {
        if (temp & 1) {
            result += power;
        }
        power *= 10;
        temp >>= 1;
    }
    
    return result;
}
