/*
 * A005205  Coding Fibonacci numbers.
 * https://oeis.org/A005205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1) * 3^Fib(n-1) + a(n-2), a(1)=1, a(2)=3
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow3(unsigned long long exp) {
    seq_t result = 1;
    seq_t base = 3;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 3;
    
    seq_t prev2 = 1;
    seq_t prev1 = 3;
    
    unsigned long long fib_a = 1;
    unsigned long long fib_b = 1;
    
    for (idx_t i = 3; i <= n; i++) {
        unsigned long long fib_c = fib_a + fib_b;
        seq_t power = pow3(fib_b);
        seq_t curr = prev1 * power + prev2;
        
        prev2 = prev1;
        prev1 = curr;
        fib_a = fib_b;
        fib_b = fib_c;
    }
    
    return prev1;
}
