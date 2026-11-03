/*
 * A004613  Numbers that are divisible only by primes congruent to 1 mod 4.
 * https://oeis.org/A004613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   Numbers divisible only by primes ≡ 1 (mod 4), by trial division
 * Range:     u8 n <= 34, u16 n <= 6420, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9258 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_only_primes_1mod4(seq_t num) {
    if (num == 1) return true;
    
    seq_t n = num;
    
    if (n % 2 == 0) {
        return false;
    }
    
    if (n % 3 == 0) {
        return false;
    }
    
    for (seq_t p = 5; p * p <= n; p += 2) {
        if (n % p == 0) {
            if (p % 4 != 1) {
                return false;
            }
            while (n % p == 0) {
                n /= p;
            }
        }
    }
    
    // Remaining n is either 1 or a prime factor
    if (n > 1 && n % 4 != 1) {
        return false;
    }
    
    return true;
}

seq_t a(idx_t n) {
    seq_t target = (seq_t)n;
    seq_t count = 0;
    seq_t num = 0;
    
    while (count < target) {
        num++;
        if (has_only_primes_1mod4(num)) {
            count++;
        }
    }
    
    return num;
}
