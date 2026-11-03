/*
 * A004619  Numbers divisible only by primes congruent to 1 mod 7.
 * https://oeis.org/A004619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 997 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n-th positive integer whose prime factors are all congruent to 1 mod 7 */

#include <stdbool.h>

static bool has_only_primes_1mod7(seq_t num) {
    if (num == 1) return true;
    
    seq_t small_primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23};
    for (int i = 0; i < 9; i++) {
        if (num % small_primes[i] == 0) return false;
    }
    
    seq_t temp = num;
    for (seq_t p = 29; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            if (p % 7 != 1) return false;
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    
    if (temp > 1 && temp % 7 != 1) {
        return false;
    }
    
    return true;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t candidate = 0;
    
    while (count < (seq_t)n) {
        candidate++;
        if (has_only_primes_1mod7(candidate)) {
            count++;
        }
    }
    
    return candidate;
}
