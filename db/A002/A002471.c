/*
 * A002471  Number of partitions of n into a prime and a square.
 * https://oeis.org/A002471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) counts partitions of n into a prime and a square; a(n) =
 *            #{p prime, s >= 0 : n = p + s^2}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime(uint64_t p) {
    if (p < 2) return false;
    if (p == 2) return true;
    if (p % 2 == 0) return false;
    if (p < 9) return true;
    if (p % 3 == 0) return false;
    
    for (uint64_t i = 5; i * i <= p; i += 6) {
        if (p % i == 0 || p % (i + 2) == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    
    for (idx_t s = 0; s * s < n; ++s) {
        idx_t square = s * s;
        idx_t prime_candidate = n - square;
        
        if (is_prime((uint64_t)prime_candidate)) {
            count++;
        }
    }
    
    return count;
}
