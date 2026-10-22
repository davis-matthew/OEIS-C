/*
 * A005203  Fibonacci numbers (or rabbit sequence) converted to decimal.
 * https://oeis.org/A005203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1) * 2^F(n-1) + a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   power_2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power_2.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    
    seq_t a_prev2 = (seq_t)0;
    seq_t a_prev1 = (seq_t)1;
    
    seq_t fib_prev2 = (seq_t)0;
    seq_t fib_prev1 = (seq_t)1;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t fib_curr = fib_prev2 + fib_prev1;
        seq_t power = power_2(fib_prev1);
        seq_t a_curr = a_prev1 * power + a_prev2;
        
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
        fib_prev2 = fib_prev1;
        fib_prev1 = fib_curr;
    }
    
    return a_prev1;
}
