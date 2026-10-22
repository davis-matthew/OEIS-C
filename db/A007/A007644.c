/*
 * A007644  Primes not of form | 3^a +- 2^b | where a, b are nonnegative
 *          integers.
 * https://oeis.org/A007644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes not of form |3^a ± 2^b|; search through primes and filter */

#include <stdbool.h>
#include "../utils/is_prime.h"

static bool is_power_of_2(uint64_t x) {
    return x > 0 && (x & (x - 1)) == 0;
}

static bool is_forbidden_form(uint64_t p) {
    uint64_t pow3 = 1;
    
    for (int a = 0; a < 42; a++) {
        // Case 1: p = 3^a + 2^b => 2^b = p - 3^a
        if (p > pow3 && is_power_of_2(p - pow3)) {
            return true;
        }
        
        // Case 2: p = 3^a - 2^b => 2^b = 3^a - p
        if (pow3 > p && is_power_of_2(pow3 - p)) {
            return true;
        }
        
        // Case 3: p = 2^b - 3^a => 2^b = p + 3^a
        if (pow3 <= UINT64_MAX - p && is_power_of_2(p + pow3)) {
            return true;
        }
        
        pow3 *= 3;
    }
    
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t candidate = 2;
    
    while (true) {
        if (is_prime(candidate) && !is_forbidden_form(candidate)) {
            count++;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        candidate++;
    }
}
