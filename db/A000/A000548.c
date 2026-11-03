/*
 * A000548  Squares that are not the sum of 2 nonzero squares.
 * https://oeis.org/A000548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m_n^2, where m_n is the n-th nonhypotenuse number
 * Range:     u8 n <= 11, u16 n <= 129, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 851 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_nonhypotenuse(uint64_t m) {
    uint64_t temp = m;
    
    // Remove factors of 2
    while (temp % 2 == 0) {
        temp /= 2;
    }
    
    // Check all odd factors
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        while (temp % p == 0) {
            // If p ≡ 1 (mod 4), m is a hypotenuse number
            if (p % 4 == 1) {
                return false;
            }
            temp /= p;
        }
    }
    
    // Check remaining prime factor
    if (temp > 1 && temp % 4 == 1) {
        return false;
    }
    
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t m = 0;
    
    while (count < n) {
        m++;
        if (is_nonhypotenuse(m)) {
            count++;
        }
    }
    
    seq_t result = (seq_t)m;
    return result * result;
}
