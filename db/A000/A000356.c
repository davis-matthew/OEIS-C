/*
 * A000356  Number of rooted cubic maps with 2n nodes and a distinguished
 *          Hamiltonian cycle: (2n)!(2n+1)! / (n!^2*(n+1)!(n+2)!).
 * https://oeis.org/A000356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   a151_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a151_gcd.h"

/* Recurrence: a(n) = 4*(2*n-1)*(2*n+1)*a(n-1) / ((n+2)*(n+1)); a(1) = 1 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    for (idx_t i = 2; i <= n; i++) {
        seq_t num = (seq_t)4 * (seq_t)(2*i-1) * (seq_t)(2*i+1);
        seq_t den = (seq_t)(i+2) * (seq_t)(i+1);
        
        seq_t g = a151_gcd(num, den);
        num /= g;
        den /= g;
        
        g = a151_gcd(result, den);
        result /= g;
        den /= g;
        
        result = result * num;
    }
    return result;
}
