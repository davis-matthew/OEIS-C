/*
 * A004614  Numbers that are divisible only by primes congruent to 3 mod 4.
 * https://oeis.org/A004614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th number whose prime factors are all congruent to
 *            3 mod 4
 * Range:     u8 n <= 61, u16 n <= 9973, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9950 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_valid_seq(seq_t n) {
    if (n == 1) return true;
    
    seq_t temp = n;
    
    // 2 ≡ 2 (mod 4), so if divisible by 2, invalid
    if (temp % 2 == 0) return false;
    
    // Check odd divisors
    for (seq_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            if (p % 4 != 3) return false;
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    
    // temp is now either 1 or a prime > sqrt(n)
    if (temp > 1 && temp % 4 != 3) return false;
    
    return true;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t candidate = 0;
    
    while (count < (seq_t)n) {
        candidate++;
        if (is_valid_seq(candidate)) {
            count++;
        }
    }
    
    return candidate;
}
