/*
 * A001124  Primes with 5 as smallest primitive root.
 * https://oeis.org/A001124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   n-th prime p where 5 is the smallest primitive root modulo p
 * Range:     u8 n <= 8, u16 n <= 932, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_primitive_root, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_primitive_root.h"
#include "../utils/powmod.h"

static bool is_prime(uint64_t num) {
    if (num < 2) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;
    if (num < 9) return true;
    if (num % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p = 2;
    
    while (count < n) {
        if (is_prime(p)) {
            if (is_primitive_root(5, p)) {
                bool is_smallest = true;
                for (uint64_t g = 2; g < 5; g++) {
                    if (is_primitive_root(g, p)) {
                        is_smallest = false;
                        break;
                    }
                }
                if (is_smallest) {
                    count++;
                    if (count == n) {
                        return (seq_t)p;
                    }
                }
            }
        }
        p++;
    }
    
    return 0;
}
