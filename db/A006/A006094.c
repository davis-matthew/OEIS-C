/*
 * A006094  Products of 2 successive primes.
 * https://oeis.org/A006094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A000040(n) * A000040(n+1), product of n-th and (n+1)-th
 *            primes
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6542, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

static void nth_and_next_prime(idx_t n, uint64_t *p1, uint64_t *p2) {
    uint64_t count = 0;
    uint64_t candidate = 2;
    
    while (count <= n) {
        if (is_prime(candidate)) {
            count++;
            if (count == n) {
                *p1 = candidate;
            }
            if (count == n + 1) {
                *p2 = candidate;
            }
        }
        candidate++;
    }
}

seq_t a(idx_t n) {
    uint64_t p1, p2;
    nth_and_next_prime(n, &p1, &p2);
    
    return (seq_t)p1 * (seq_t)p2;
}
