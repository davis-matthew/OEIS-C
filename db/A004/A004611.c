/*
 * A004611  Divisible only by primes congruent to 1 mod 3.
 * https://oeis.org/A004611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 5883, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9680 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_valid(seq_t m) {
    if (m == 1) return true;
    
    seq_t n = m;
    
    for (seq_t p = 2; n / p >= p; p++) {
        if (n % p == 0) {
            if (p % 3 != 1) return false;
            while (n % p == 0) {
                n /= p;
            }
        }
    }
    
    if (n > 1 && n % 3 != 1) return false;
    
    return true;
}

/* a(n) = n-th positive integer whose prime divisors are all ≡ 1 (mod 3) */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    idx_t count = 1;
    seq_t candidate = 2;
    
    while (true) {
        if (is_valid(candidate)) {
            count++;
            if (count == n) return candidate;
        }
        candidate++;
    }
}
