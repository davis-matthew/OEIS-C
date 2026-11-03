/*
 * A004617  Divisible only by primes congruent to 3 mod 5.
 * https://oeis.org/A004617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers divisible only by primes congruent to 3 mod 5
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 875 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool all_prime_factors_3_mod_5(seq_t k) {
    seq_t n = k;
    seq_t p = 2;
    
    while (p <= n / p) {
        if (n % p == 0) {
            if (p % 5 != 3) {
                return false;
            }
            while (n % p == 0) {
                n /= p;
            }
        }
        p++;
    }
    
    if (n > 1 && n % 5 != 3) {
        return false;
    }
    
    return true;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t k = 0;
    
    while (count < n) {
        k++;
        if (all_prime_factors_3_mod_5(k)) {
            count++;
        }
    }
    
    return k;
}
