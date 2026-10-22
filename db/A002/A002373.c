/*
 * A002373  Smallest prime in decomposition of 2n into sum of two odd primes.
 * https://oeis.org/A002373
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = smallest odd prime p such that 2n = p + q, where both p
 *            and q are odd primes
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    uint64_t target = n * 2;
    
    for (uint64_t p = 3; p < target; p += 2) {
        if (is_prime(p) && is_prime(target - p)) {
            return (seq_t)p;
        }
    }
    
    return 0;
}
