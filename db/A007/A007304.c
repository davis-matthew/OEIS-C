/*
 * A007304  Sphenic numbers: products of 3 distinct primes.
 * https://oeis.org/A007304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   Sphenic numbers = products of exactly 3 distinct primes; a(n) is
 *            the n-th such number
 * Range:     u8 n <= 25, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9670 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_sphenic(seq_t num) {
    seq_t temp = num;
    int distinct_count = 0;
    
    // Find all prime factors up to sqrt(num)
    for (seq_t p = 2; p * p <= num; p++) {
        if (temp % p == 0) {
            distinct_count++;
            if (distinct_count > 3) return false;
            
            temp /= p;
            
            // Check that p divides exactly once
            if (temp % p == 0) return false;
        }
    }
    
    // If temp > 1, it's a prime factor greater than sqrt(num)
    if (temp > 1) {
        distinct_count++;
    }
    
    return distinct_count == 3;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    
    for (seq_t candidate = 30; ; candidate++) {
        if (is_sphenic(candidate)) {
            if (++count == n) {
                return candidate;
            }
        }
    }
}
