/*
 * A004690  Fibonacci numbers written in base 7.
 * https://oeis.org/A004690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = base-7 representation of the n-th Fibonacci number, read
 *            as decimal
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 41, u64 n <= 79, u128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t fib;
    if (n == 0) {
        fib = 0;
    } else if (n == 1) {
        fib = 1;
    } else {
        seq_t fib0 = 0, fib1 = 1;
        for (idx_t i = 2; i <= n; i++) {
            seq_t temp = fib0 + fib1;
            fib0 = fib1;
            fib1 = temp;
        }
        fib = fib1;
    }
    
    seq_t result = 0;
    seq_t power = 1;
    seq_t temp = fib;
    
    while (temp > 0) {
        seq_t digit = temp % 7;
        result = result + digit * power;
        power = power * 10;
        temp = temp / 7;
    }
    
    return result;
}
