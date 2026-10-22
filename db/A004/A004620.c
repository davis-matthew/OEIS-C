/*
 * A004620  Divisible only by primes congruent to 2 mod 7.
 * https://oeis.org/A004620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   numbers whose prime factors are all congruent to 2 mod 7
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_valid(seq_t m) {
    if (m == 1) return true;
    
    // Divide out 2
    while (m % 2 == 0) m /= 2;
    
    // Check all odd divisors
    for (seq_t p = 3; p * p <= m; p += 2) {
        while (m % p == 0) {
            if (p % 7 != 2) return false;
            m /= p;
        }
    }
    
    // If m > 1, it's a prime factor
    if (m > 1 && m % 7 != 2) return false;
    
    return true;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t candidate = 0;
    
    while (count < (seq_t)n) {
        candidate++;
        if (is_valid(candidate)) count++;
    }
    
    return candidate;
}
