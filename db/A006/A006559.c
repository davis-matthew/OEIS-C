/*
 * A006559  Short period primes: the decimal expansion of 1/p has period less
 *          than p-1, but greater than zero.
 * https://oeis.org/A006559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, base
 * Formula:   a(n) = n-th prime p where ord_p(10) in (0, p-1)
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 844 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t multiplicative_order(uint64_t a, uint64_t p) {
    uint64_t result = a % p;
    if (result == 0) return 0;
    uint64_t order = 1;
    while (result != 1) {
        result = (result * a) % p;
        order++;
    }
    return order;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p = 3;
    
    while (true) {
        if (is_prime_u64(p)) {
            uint64_t order = multiplicative_order(10, p);
            if (order > 0 && order < p - 1) {
                count++;
                if (count == n) {
                    return (seq_t)p;
                }
            }
        }
        p += 2;
    }
}
