/*
 * A000379  Numbers where total number of 1-bits in the exponents of their
 *          prime factorization is even; a 2-way classification of integers:
 *          complement of A000028.
 * https://oeis.org/A000379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9095 (2026-11-01)
 * Helpers:   popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterates through positive integers and returns those with even total 1-bits in prime exponents */

#include <stdbool.h>
#include "../utils/popcount.h"

static bool in_a000379(uint64_t m) {
    if (m == 1) return true;
    
    uint64_t temp = m;
    int total_bits = 0;
    
    for (uint64_t p = 2; p * p <= temp; p++) {
        int exp = 0;
        while (temp % p == 0) {
            exp++;
            temp /= p;
        }
        if (exp > 0) {
            total_bits += popcount((uint64_t)exp);
        }
    }
    
    if (temp > 1) {
        total_bits += 1;
    }
    
    return total_bits % 2 == 0;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t m = 0;
    
    while (count < (seq_t)n) {
        m++;
        if (in_a000379((uint64_t)m)) {
            count++;
        }
    }
    
    return m;
}
