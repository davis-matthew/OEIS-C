/*
 * A007619  Wilson quotients: ((p-1)! + 1)/p where p is the n-th prime.
 * https://oeis.org/A007619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = ((p-1)! + 1)/p where p is the n-th prime
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"


seq_t a(idx_t n) {
    // Find the n-th prime
    uint64_t count = 0;
    uint64_t candidate = 2;
    uint64_t p = 0;
    
    while (count < n) {
        if (is_prime(candidate)) {
            count++;
            if (count == n) {
                p = candidate;
                break;
            }
        }
        candidate++;
    }
    
    // Compute (p-1)!
    seq_t factorial = 1;
    for (uint64_t i = 2; i < p; i++) {
        factorial = factorial * (seq_t)i;
    }
    
    // Compute ((p-1)! + 1) / p
    seq_t result = (factorial + 1) / (seq_t)p;
    
    return result;
}
