/*
 * A005528  Størmer numbers or arc-cotangent irreducible numbers: numbers k
 *          such that the largest prime factor of k^2 + 1 is >= 2*k.
 * https://oeis.org/A005528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 180, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: Iterate k >= 1; count where largest prime factor of k^2 + 1 >= 2*k */

#include <stdint.h>

static uint64_t largest_prime_factor(uint64_t n) {
    uint64_t lpf = 1;
    
    while (n % 2 == 0) {
        lpf = 2;
        n /= 2;
    }
    
    for (uint64_t i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            lpf = i;
            n /= i;
        }
    }
    
    if (n > 1) {
        lpf = n;
    }
    
    return lpf;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 0;
    
    while (count < n) {
        k++;
        uint64_t k_sq_plus_1 = (uint64_t)k * k + 1;
        uint64_t lpf = largest_prime_factor(k_sq_plus_1);
        
        if (lpf >= 2 * k) {
            count++;
        }
    }
    
    return (seq_t)k;
}
