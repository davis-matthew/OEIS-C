/*
 * A002035  Numbers that contain primes to odd powers only.
 * https://oeis.org/A002035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   n-th number whose prime factorization contains only odd
 *            exponents
 * Range:     u8 n <= 182, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 86 terms through n = 988 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_all_odd_exponents(seq_t m) {
    if (m == 1) return false;
    
    seq_t remaining = m;
    
    for (seq_t p = 2; p <= m / p; p++) {
        int exponent = 0;
        while (remaining % p == 0) {
            exponent++;
            remaining /= p;
        }
        if (exponent > 0 && exponent % 2 == 0) {
            return false;
        }
    }
    
    return true;
}

seq_t a(idx_t n) {
    seq_t target = (seq_t)n;
    seq_t count = 0;
    seq_t num = 1;
    
    while (count < target) {
        num++;
        if (has_all_odd_exponents(num)) {
            count++;
        }
    }
    
    return num;
}
