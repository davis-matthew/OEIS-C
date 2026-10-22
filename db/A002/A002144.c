/*
 * A002144  Pythagorean primes: primes of the form 4*k + 1.
 * https://oeis.org/A002144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 3257, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 4*k + 1 (Pythagorean primes) */

#include <stdbool.h>

static bool is_prime(uint64_t p) {
    if (p < 2) return false;
    if (p == 2) return true;
    if (p % 2 == 0) return false;
    if (p < 9) return true;
    if (p % 3 == 0) return false;
    
    for (uint64_t i = 5; i * i <= p; i += 6) {
        if (p % i == 0 || p % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t candidate = 5;
    
    while (true) {
        if (is_prime(candidate)) {
            count++;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        candidate += 4;
    }
}
