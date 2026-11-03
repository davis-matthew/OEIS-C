/*
 * A002312  Arc-cotangent reducible numbers or non-Størmer numbers: largest
 *          prime factor of k^2 + 1 is less than 2*k.
 * https://oeis.org/A002312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 125, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 970 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t largest_prime_factor(seq_t n) {
    seq_t largest = 0;
    
    while (n % 2 == 0) {
        largest = 2;
        n /= 2;
    }
    
    for (seq_t i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            largest = i;
            n /= i;
        }
    }
    
    if (n > 1) {
        largest = n;
    }
    
    return largest;
}

/* Numbers k: largest prime factor of k^2 + 1 < 2*k */

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 0;
    
    while (count < n) {
        k++;
        seq_t k_sq = k * k;
        seq_t k_sq_plus_1 = k_sq + 1;
        seq_t lpf = largest_prime_factor(k_sq_plus_1);
        
        if (lpf < 2 * k) {
            count++;
        }
    }
    
    return k;
}
