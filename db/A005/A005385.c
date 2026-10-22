/*
 * A005385  Safe primes p: (p-1)/2 is also prime.
 * https://oeis.org/A005385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 474, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Search for the n-th safe prime: prime p where (p-1)/2 is also prime */

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (uint64_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t candidate = 2;
    
    while (1) {
        if (is_prime(candidate)) {
            uint64_t sophie_germain = (candidate - 1) / 2;
            if (is_prime(sophie_germain)) {
                count++;
                if (count == n) {
                    return (seq_t)candidate;
                }
            }
        }
        if (candidate == 2) {
            candidate = 3;
        } else {
            candidate += 2;
        }
    }
}
