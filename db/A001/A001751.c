/*
 * A001751  Primes together with primes multiplied by 2.
 * https://oeis.org/A001751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth term in the sorted sequence of primes and
 *            2*primes
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime(uint64_t p) {
    if (p <= 1) return false;
    if (p <= 3) return true;
    if (p % 2 == 0 || p % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= p; i += 6) {
        if (p % i == 0 || p % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    uint64_t found = 0;
    for (uint64_t val = 2; ; val++) {
        bool in_seq = false;
        
        // Check if val is prime
        if (is_prime(val)) {
            in_seq = true;
        }
        // Check if val = 2*p for some prime p
        else if (val % 2 == 0) {
            uint64_t p = val / 2;
            if (is_prime(p)) {
                in_seq = true;
            }
        }
        
        if (in_seq) {
            found++;
            if (found == n) {
                return (seq_t)val;
            }
        }
    }
}
