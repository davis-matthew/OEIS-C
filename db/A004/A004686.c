/*
 * A004686  Fibonacci numbers written in base 3.
 * https://oeis.org/A004686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Fibonacci numbers written in base 3
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 46, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t f0 = 0, f1 = 1;
    
    for (idx_t i = 0; i < n; i++) {
        seq_t temp = f0 + f1;
        f0 = f1;
        f1 = temp;
    }
    
    seq_t fib = f0;
    
    seq_t result = 0;
    seq_t power = 1;
    
    while (fib > 0) {
        seq_t digit = fib % 3;
        result = result + digit * power;
        power = power * 10;
        fib = fib / 3;
    }
    
    return result;
}
