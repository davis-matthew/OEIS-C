/*
 * A001097  Twin primes.
 * https://oeis.org/A001097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, core, nice
 * Formula:   a(n) = the n-th number that is part of a twin prime pair
 * Range:     u8 n <= 33, u16 n <= 1719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static bool is_twin_prime(seq_t p) {
    if (p < 3) return false;
    
    uint64_t p64 = (uint64_t)p;
    if (!is_prime(p64)) return false;
    
    if (is_prime(p64 + 2)) return true;
    if (is_prime(p64 - 2)) return true;
    
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = 2;
    
    while (count < n) {
        candidate++;
        if (is_twin_prime(candidate)) {
            count++;
        }
    }
    
    return candidate;
}
