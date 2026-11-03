/*
 * A004618  Divisible only by primes congruent to 4 mod 5.
 * https://oeis.org/A004618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Sequence of positive integers divisible only by primes congruent
 *            to 4 mod 5. Search for the n-th such number
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 854 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_only_primes_4mod5(seq_t num) {
    if (num == 1) return true;
    
    seq_t n = num;
    
    // Check divisor 2
    if (n % 2 == 0) return false;
    
    // Check odd divisors
    for (seq_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            if (p % 5 != 4) return false;
            while (n % p == 0) {
                n /= p;
            }
        }
    }
    
    // Check remaining factor
    if (n > 1 && n % 5 != 4) return false;
    
    return true;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t candidate = 0;
    
    while (count < (seq_t)n) {
        candidate++;
        if (has_only_primes_4mod5(candidate)) {
            count++;
        }
    }
    
    return candidate;
}
