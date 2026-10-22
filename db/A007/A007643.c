/*
 * A007643  Primes not of form | 3^x - 2^y |.
 * https://oeis.org/A007643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Primes not expressible as |3^x - 2^y| for any non-negative
 *            integers x, y
 * Range:     u8 n <= 29, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_power_of_3, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_power_of_3.h"

static bool is_power_of_2(uint64_t n) {
    if (n == 0) return false;
    return (n & (n - 1)) == 0;
}

static bool can_be_expressed(uint64_t p) {
    // Try powers of 3: check if p = |3^x - 2^y|
    for (uint64_t pow3 = 1; pow3 < (1ULL << 50); pow3 *= 3) {
        if (pow3 > p && is_power_of_2(pow3 - p)) return true;
        if (is_power_of_2(p + pow3)) return true;
        if (pow3 > UINT64_MAX / 3) break;
    }
    
    // Try powers of 2: check if p = |3^x - 2^y|
    for (uint64_t pow2 = 1; pow2 < (1ULL << 50); pow2 *= 2) {
        if (pow2 > p && is_power_of_3(pow2 - p)) return true;
        if (is_power_of_3(p + pow2)) return true;
        if (pow2 > UINT64_MAX / 2) break;
    }
    
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t candidate = 2;
    
    while (count < n) {
        if (is_prime(candidate) && !can_be_expressed(candidate)) {
            count++;
            if (count == n) return (seq_t)candidate;
        }
        candidate++;
    }
    
    return 0;
}
