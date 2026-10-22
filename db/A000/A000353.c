/*
 * A000353  Primes p == 7, 19, 23 (mod 40) such that (p-1)/2 is also prime.
 * https://oeis.org/A000353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A000355(n)+1, where A000355(n) are primes p ≡ 7, 19, 23
 *            (mod 40) with (p-1)/2 also prime
 * Range:     u8 n <= 6, u16 n <= 226, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = 7;
    
    while (1) {
        seq_t mod = candidate % 40;
        if ((mod == 7 || mod == 19 || mod == 23) &&
            is_prime((uint64_t)candidate) && 
            is_prime((uint64_t)((candidate - 1) / 2))) {
            count++;
            if (count == n) {
                return candidate;
            }
        }
        candidate += 2;
    }
}
