/*
 * A004621  Divisible only by primes congruent to 3 mod 7.
 * https://oeis.org/A004621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Search: nth positive integer divisible only by primes p ≡ 3 (mod 7) */

static bool is_valid(seq_t num) {
    if (num == 1) return true;
    
    seq_t temp = num;
    
    for (seq_t p = 2; p <= temp / p; p++) {
        if (temp % p == 0) {
            if (p % 7 != 3) {
                return false;
            }
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    
    if (temp > 1 && temp % 7 != 3) {
        return false;
    }
    
    return true;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t num = 0;
    
    while (count < n) {
        num++;
        if (is_valid(num)) {
            count++;
        }
    }
    
    return num;
}
