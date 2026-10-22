/*
 * A001248  Squares of primes.
 * https://oeis.org/A001248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000040(n)^2, where A000040(n) is the n-th prime
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6542, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime(uint64_t x) {
    if (x < 2) return false;
    if (x == 2) return true;
    if (x % 2 == 0) return false;
    for (uint64_t i = 3; i * i <= x; i += 2) {
        if (x % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)4;
    }
    uint64_t count = 1;
    uint64_t candidate = 3;
    while (count < n) {
        if (is_prime(candidate)) {
            count++;
        }
        if (count < n) {
            candidate += 2;
        }
    }
    seq_t prime = (seq_t)candidate;
    return prime * prime;
}
