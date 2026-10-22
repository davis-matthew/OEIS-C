/*
 * A004688  Fibonacci numbers written in base 5.
 * https://oeis.org/A004688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Fibonacci(n) in base 5, interpreted as decimal
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t fib0 = 0, fib1 = 1;
    
    for (idx_t i = 1; i <= n; i++) {
        seq_t temp = fib0 + fib1;
        fib0 = fib1;
        fib1 = temp;
    }
    
    seq_t fib_n = fib0;
    seq_t result = 0;
    seq_t power = 1;
    
    while (fib_n > 0) {
        seq_t digit = fib_n % 5;
        result = result + digit * power;
        power = power * 10;
        fib_n = fib_n / 5;
    }
    
    return result;
}
