/*
 * A006881  Squarefree semiprimes: Numbers that are the product of two
 *          distinct primes.
 * https://oeis.org/A006881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) is the n-th squarefree semiprime (product of exactly two
 *            distinct primes), computed by trial division
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9893 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_squarefree_semiprime(uint64_t n) {
    if (n < 6) return false;
    
    int distinct_primes = 0;
    uint64_t temp = n;
    
    for (uint64_t p = 2; p * p <= n; p++) {
        if (temp % p == 0) {
            distinct_primes++;
            if (distinct_primes > 2) return false;
            
            temp /= p;
            if (temp % p == 0) return false;
        }
    }
    
    if (temp > 1) {
        distinct_primes++;
    }
    
    return distinct_primes == 2;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t candidate = 6;
    
    while (true) {
        if (is_squarefree_semiprime(candidate)) {
            count++;
            if (count == n) return (seq_t)candidate;
        }
        candidate++;
    }
}
