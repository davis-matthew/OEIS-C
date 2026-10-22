/*
 * A003632  Inert rational primes in Q(sqrt 7), or, 7 is not a square mod p.
 * https://oeis.org/A003632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   n-th prime p such that p ≡ 5, 11, 13, 15, 17, 23 (mod 28)
 * Range:     u8 n <= 28, u16 n <= 3271, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    
    for (uint64_t candidate = 2; ; candidate++) {
        if (is_prime(candidate)) {
            uint64_t mod = candidate % 28;
            if (mod == 5 || mod == 11 || mod == 13 || mod == 15 || mod == 17 || mod == 23) {
                count++;
                if (count == n) {
                    return (seq_t)candidate;
                }
            }
        }
    }
}
