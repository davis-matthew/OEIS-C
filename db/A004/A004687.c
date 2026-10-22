/*
 * A004687  Fibonacci numbers written in base 4.
 * https://oeis.org/A004687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = F(n) in base 4, read as decimal
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 57, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    // Compute Fibonacci number F(n)
    seq_t fib_prev = 0, fib_curr = 1;
    for (idx_t i = 2; i <= n; i++) {
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    
    // Convert to base 4 and read as decimal
    seq_t result = 0;
    seq_t power_of_10 = 1;
    while (fib_curr > 0) {
        seq_t digit = fib_curr % 4;
        result = result + digit * power_of_10;
        power_of_10 = power_of_10 * 10;
        fib_curr = fib_curr / 4;
    }
    
    return result;
}
