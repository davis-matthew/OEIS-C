/*
 * A004622  Divisible only by primes congruent to 4 mod 7.
 * https://oeis.org/A004622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers divisible only by primes congruent to 4 mod 7; search
 *            method
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_valid(seq_t m) {
    if (m == 1) return true;
    
    if (m % 2 == 0) return false;
    
    seq_t temp = m;
    
    for (seq_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            if (p % 7 != 4) return false;
            while (temp % p == 0) temp /= p;
        }
    }
    
    if (temp > 1 && temp % 7 != 4) return false;
    
    return true;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    idx_t count = 1;
    for (seq_t m = 3; ; m += 2) {
        if (is_valid(m)) {
            count++;
            if (count == n) return m;
        }
    }
}
