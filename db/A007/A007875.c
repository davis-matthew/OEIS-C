/*
 * A007875  Number of ways of writing n as p*q, with p <= q, gcd(p, q) = 1.
 * https://oeis.org/A007875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2^(omega(n)-1) for n > 1, a(1) = 1, where omega(n) is the
 *            number of distinct prime divisors
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pw2.h"

seq_t a(idx_t n) {
    if (n == 1) {
        return 1;
    }
    
    // Count distinct prime divisors of n
    uint64_t m = n;
    int distinct_primes = 0;
    
    // Check for factor 2
    if (m % 2 == 0) {
        distinct_primes++;
        while (m % 2 == 0) {
            m /= 2;
        }
    }
    
    // Check for odd factors from 3 onwards
    for (uint64_t i = 3; i * i <= m; i += 2) {
        if (m % i == 0) {
            distinct_primes++;
            while (m % i == 0) {
                m /= i;
            }
        }
    }
    
    // If m > 1, then it's a prime factor
    if (m > 1) {
        distinct_primes++;
    }
    
    // Return 2^(distinct_primes - 1)
    return pw2((idx_t)(distinct_primes - 1));
}
