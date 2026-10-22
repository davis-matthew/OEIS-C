/*
 * A004693  Fibonacci numbers written in base 12. (Next term contains a
 *          non-decimal character.)
 * https://oeis.org/A004693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Fibonacci(n) converted to base 12, digits interpreted as decimal
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Compute the n-th Fibonacci number
    seq_t fib = 0, next = 1;
    
    for (idx_t i = 0; i < n; i++) {
        seq_t t = fib + next;
        fib = next;
        next = t;
    }
    
    // Convert to base 12 and interpret as decimal
    seq_t result = 0;
    seq_t power = 1;
    seq_t num = fib;
    
    while (num > 0) {
        seq_t digit = num % 12;
        result += digit * power;
        power *= 10;
        num /= 12;
    }
    
    return result;
}
