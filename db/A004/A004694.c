/*
 * A004694  Fibonacci numbers written in base 13. (Next term contains a
 *          non-decimal character.)
 * https://oeis.org/A004694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Fibonacci(n) converted to base 13, then interpreted as
 *            base 10
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Compute the nth Fibonacci number
    seq_t fib = 0, next = 1;
    
    for (idx_t i = 0; i < n; i++) {
        seq_t sum = fib + next;
        fib = next;
        next = sum;
    }
    
    // Convert fib to base 13 and interpret as base 10
    seq_t result = 0;
    seq_t power = 1;
    seq_t temp = fib;
    
    while (temp > 0) {
        seq_t digit = temp % 13;
        result = result + digit * power;
        power = power * 10;
        temp = temp / 13;
    }
    
    return result;
}
