/*
 * A005598  a(n) = 1 + Sum_{i=1..n} (n-i+1)*phi(i).
 * https://oeis.org/A005598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 1 + Sum_{i=1..n} (n-i+1)*phi(i)
 * Range:     u8 n <= 12, u16 n <= 85, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t phi(idx_t n) {
    seq_t result = (seq_t)n;
    idx_t temp_n = n;
    
    for (idx_t p = 2; p * p <= temp_n; p++) {
        if (temp_n % p == 0) {
            while (temp_n % p == 0) {
                temp_n /= p;
            }
            result = result - result / p;
        }
    }
    
    if (temp_n > 1) {
        result = result - result / temp_n;
    }
    
    return result;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        seq_t coeff = ((seq_t)n + 1) - (seq_t)i;
        result += coeff * phi(i);
    }
    return result;
}
