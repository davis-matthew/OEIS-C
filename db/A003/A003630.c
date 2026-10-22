/*
 * A003630  Inert rational primes in Q[sqrt(3)].
 * https://oeis.org/A003630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p ≡ 5, 7 (mod 12)
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    if (n < 9) return true;
    if (n % 3 == 0) return false;
    
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t candidate = 5;
    
    while (true) {
        if (is_prime(candidate)) {
            uint64_t mod = candidate % 12;
            if (mod == 5 || mod == 7) {
                count++;
                if (count == n) {
                    return (seq_t)candidate;
                }
            }
        }
        candidate += 2;
    }
}
