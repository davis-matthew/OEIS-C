/*
 * A004691  Fibonacci numbers written in base 8.
 * https://oeis.org/A004691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = interpret Fibonacci(n) base-8 digits as base-10
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 43, u64 n <= 85, u128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    
    // Compute Fibonacci(n)
    seq_t f0 = 0, f1 = 1;
    for (idx_t i = 2; i <= n; i++) {
        seq_t f_next = f0 + f1;
        f0 = f1;
        f1 = f_next;
    }
    
    // Convert f1 (Fibonacci(n)) to base 8 digits, interpret as base 10
    seq_t result = 0;
    seq_t power = 1;
    seq_t fib_n = f1;
    
    while (fib_n > 0) {
        seq_t digit = fib_n % 8;
        result = result + digit * power;
        power = power * 10;
        fib_n = fib_n / 8;
    }
    
    return result;
}
