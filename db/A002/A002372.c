/*
 * A002372  Goldbach conjecture: number of decompositions of 2n into ordered
 *          sums of two odd primes.
 * https://oeis.org/A002372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = A010051(n) + 2*A061357(n) for n > 2; a(1) = a(2) = 0
 * Range:     u8 n <= 1364, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_odd_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_odd_prime.h"

seq_t a(idx_t n) {
    if (n <= 2) return 0;
    
    uint64_t two_n = n * 2;
    seq_t count = 0;
    
    if (is_odd_prime(n)) {
        count++;
    }
    
    for (uint64_t p = 3; p < two_n / 2; p += 2) {
        if (is_odd_prime(p)) {
            uint64_t q = two_n - p;
            if (is_odd_prime(q)) {
                count += 2;
            }
        }
    }
    
    return count;
}
