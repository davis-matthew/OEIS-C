/*
 * A004692  Fibonacci numbers written in base 9.
 * https://oeis.org/A004692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = F(n) in base 9
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 45, u64 n <= 89, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t fib_prev = 0;
    seq_t fib_curr = 0;
    
    if (n > 0) {
        fib_curr = 1;
        for (idx_t i = 1; i < n; i++) {
            seq_t fib_next = fib_curr + fib_prev;
            fib_prev = fib_curr;
            fib_curr = fib_next;
        }
    }
    
    // Convert fib_curr from base 10 to base 9 representation
    seq_t result = 0;
    seq_t power = 1;
    
    while (fib_curr > 0) {
        seq_t digit = fib_curr % 9;
        result = result + digit * power;
        power = power * 10;
        fib_curr = fib_curr / 9;
    }
    
    return result;
}
