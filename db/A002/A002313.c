/*
 * A002313  Primes congruent to 1 or 2 modulo 4; or, primes of form x^2 + y^2;
 *          or, -1 is a square mod p.
 * https://oeis.org/A002313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) is the n-th prime p ≡ 1 (mod 4) or p = 2
 * Range:     u8 n <= 25, u16 n <= 3258, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime(uint64_t m) {
    if (m < 2) return false;
    if (m == 2) return true;
    if (m % 2 == 0) return false;
    for (uint64_t i = 3; i * i <= m; i += 2) {
        if (m % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2;
    
    idx_t count = 1;
    uint64_t candidate = 1;
    
    while (count < n) {
        candidate += 4;
        if (is_prime(candidate)) {
            count++;
        }
    }
    
    return (seq_t)candidate;
}
