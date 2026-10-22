/*
 * A007821  Primes p such that pi(p) is not prime.
 * https://oeis.org/A007821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000040(A018252(n))
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (uint64_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

static uint64_t nth_non_prime(idx_t n) {
    idx_t count = 0;
    for (uint64_t i = 1; ; i++) {
        if (!is_prime(i)) {
            count++;
            if (count == n) return i;
        }
    }
}

static uint64_t nth_prime(idx_t n) {
    if (n == 1) return 2;
    
    idx_t count = 1;
    for (uint64_t p = 3; ; p += 2) {
        if (is_prime(p)) {
            count++;
            if (count == n) return p;
        }
    }
}

seq_t a(idx_t n) {
    uint64_t k = nth_non_prime(n);
    return (seq_t)nth_prime(k);
}
