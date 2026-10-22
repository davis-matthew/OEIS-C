/*
 * A002445  Denominators of Bernoulli numbers B_{2n}.
 * https://oeis.org/A002445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, nice
 * Formula:   By von Staudt-Clausen theorem, a(n) = product of primes p where
 *            (p-1) divides 2n
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 89, u64 n <= 269,
 *            u128 n <= 1889, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime(uint64_t p) {
    if (p < 2) return false;
    if (p == 2) return true;
    if (p % 2 == 0) return false;
    if (p == 3) return true;
    if (p % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= p; i += 6) {
        if (p % i == 0 || p % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    idx_t two_n = 2 * n;
    seq_t result = 1;
    
    for (idx_t d = 1; d * d <= two_n; d++) {
        if (two_n % d == 0) {
            idx_t p = d + 1;
            if (is_prime(p)) {
                result = result * (seq_t)p;
            }
            
            if (d * d != two_n) {
                idx_t d2 = two_n / d;
                p = d2 + 1;
                if (is_prime(p)) {
                    result = result * (seq_t)p;
                }
            }
        }
    }
    
    return result;
}
