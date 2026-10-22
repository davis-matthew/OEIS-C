/*
 * A004055  ((p-1)/2)! mod p for odd primes p.
 * https://oeis.org/A004055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((p-1)/2)! mod p, where p is the nth odd prime
 * Range:     u8 n <= 53, u16 n <= 6541, u32 n <= 9131, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    // Find the nth odd prime (starting from 3)
    idx_t count = 0;
    uint64_t p = 3;
    
    while (count < n) {
        if (is_prime(p)) {
            count++;
            if (count == n) break;
        }
        p += 2;
    }
    
    // Compute ((p-1)/2)! mod p
    uint64_t k = (p - 1) / 2;
    seq_t result = 1;
    
    for (uint64_t i = 1; i <= k; i++) {
        result = (result * (seq_t)i) % (seq_t)p;
    }
    
    return result;
}
